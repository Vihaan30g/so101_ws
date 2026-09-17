// Bridges the real SO-101 Feetech servo bus to ROS 2. C++/ament_cmake
// counterpart of so101_hardware_interface (python) -- see that
// package's docstring for the full rationale on integrate-velocity /
// hold-on-error / measured-dt design. Logic here is intentionally kept
// identical to that version; only the language + Python-embedding layer
// differ.
//
// Contract:
//   Subscribes:  /joint_cmd_safe  (so101_msgs/JointCommand)  velocities, rad/s
//   Publishes:   /joint_states    (sensor_msgs/JointState)   positions, rad
//                /diagnostics     (diagnostic_msgs/DiagnosticArray)

#include <chrono>
#include <cmath>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "diagnostic_msgs/msg/diagnostic_array.hpp"
#include "diagnostic_msgs/msg/diagnostic_status.hpp"
#include "diagnostic_msgs/msg/key_value.hpp"
#include "so101_msgs/msg/joint_command.hpp"

#include "so101_hardware_interface_cpp/so_follower_bridge.hpp"

using namespace std::chrono_literals;

namespace
{
constexpr double kRadToDeg = 180.0 / M_PI;
constexpr double kDegToRad = M_PI / 180.0;
}

class HardwareInterfaceNode : public rclcpp::Node
{
public:
  HardwareInterfaceNode()
  : Node("hardware_interface_node")
  {
    port_ = this->declare_parameter<std::string>("port", "/dev/ttyACM0");
    robot_id_ = this->declare_parameter<std::string>("robot_id", "vihu2");
    venv_site_packages_ = this->declare_parameter<std::string>("venv_site_packages", "");
    control_rate_hz_ = this->declare_parameter<double>("control_rate_hz", 100.0);
    joint_cmd_timeout_ms_ = this->declare_parameter<int>("joint_cmd_timeout_ms", 100);
    overrun_ratio_ = this->declare_parameter<double>("tick_overrun_warn_ratio", 1.5);
    joint_names_ = this->declare_parameter<std::vector<std::string>>(
      "joint_names",
      {"shoulder_pan", "shoulder_lift", "elbow_flex", "wrist_flex", "wrist_roll", "gripper"});

    nominal_dt_ = 1.0 / control_rate_hz_;
    for (const auto & j : joint_names_) {
      latest_velocities_[j] = 0.0;
    }

    bridge_.initInterpreter(venv_site_packages_);
    RCLCPP_INFO(get_logger(), "Connecting to SO-101 on %s ...", port_.c_str());
    bridge_.connect(port_, robot_id_);
    RCLCPP_INFO(get_logger(), "Connected.");

    // Seed running setpoint from the real arm so the first tick doesn't jump it.
    auto obs = bridge_.getObservation();
    for (const auto & j : joint_names_) {
      auto it = obs.find(j + ".pos");
      if (it != obs.end()) {
        pos_deg_[j] = it->second;
      } else {
        RCLCPP_ERROR(
          get_logger(),
          "Joint '%s' not found in SOFollower observation. Check joint_names param.",
          j.c_str());
      }
    }

    rclcpp::QoS sensor_qos = rclcpp::SensorDataQoS();

    cmd_sub_ = this->create_subscription<so101_msgs::msg::JointCommand>(
      "/joint_cmd_safe", sensor_qos,
      std::bind(&HardwareInterfaceNode::onJointCmd, this, std::placeholders::_1));

    state_pub_ = this->create_publisher<sensor_msgs::msg::JointState>(
      "/joint_states", sensor_qos);

    diag_pub_ = this->create_publisher<diagnostic_msgs::msg::DiagnosticArray>(
      "/diagnostics", 10);

    timer_ = this->create_wall_timer(
      std::chrono::duration<double>(nominal_dt_),
      std::bind(&HardwareInterfaceNode::tick, this));
  }

  ~HardwareInterfaceNode() override
  {
    bridge_.disconnect();
  }

private:
  void onJointCmd(const so101_msgs::msg::JointCommand::SharedPtr msg)
  {
    last_cmd_stamp_ = this->get_clock()->now();
    for (size_t i = 0; i < msg->joint_names.size() && i < msg->velocities.size(); ++i) {
      const std::string & name = msg->joint_names[i];
      auto it = latest_velocities_.find(name);
      if (it != latest_velocities_.end()) {
        it->second = msg->velocities[i];
      } else {
        RCLCPP_WARN(
          get_logger(), "Received velocity for unknown joint \"%s\", ignoring.",
          name.c_str());
      }
    }
  }

  void publishDiag(uint8_t level, const std::string & message,
    const std::map<std::string, std::string> & values = {})
  {
    diagnostic_msgs::msg::DiagnosticArray arr;
    arr.header.stamp = this->get_clock()->now();
    diagnostic_msgs::msg::DiagnosticStatus status;
    status.level = level;
    status.name = "hardware_interface_node";
    status.message = message;
    for (const auto & kv : values) {
      diagnostic_msgs::msg::KeyValue keyval;
      keyval.key = kv.first;
      keyval.value = kv.second;
      status.values.push_back(keyval);
    }
    arr.status.push_back(status);
    diag_pub_->publish(arr);
  }

  void tick()
  {
    auto now_steady = std::chrono::steady_clock::now();
    double dt;
    if (!has_last_tick_) {
      dt = nominal_dt_;
      has_last_tick_ = true;
    } else {
      dt = std::chrono::duration<double>(now_steady - last_tick_time_).count();
    }
    last_tick_time_ = now_steady;

    if (dt > overrun_ratio_ * nominal_dt_) {
      publishDiag(
        diagnostic_msgs::msg::DiagnosticStatus::WARN,
        "Tick overrun: bus/loop slower than configured control_rate_hz. "
        "Consider lowering control_rate_hz in hardware_params.yaml.",
        {{"measured_dt_s", std::to_string(dt)}, {"nominal_dt_s", std::to_string(nominal_dt_)}});
    }

    bool stale = true;
    if (last_cmd_stamp_.has_value()) {
      double age_s = (this->get_clock()->now() - last_cmd_stamp_.value()).seconds();
      stale = age_s > (joint_cmd_timeout_ms_ / 1000.0);
    }

    try {
      for (const auto & j : joint_names_) {
        if (pos_deg_.find(j) == pos_deg_.end()) {continue;}
        double vel = stale ? 0.0 : latest_velocities_[j];
        pos_deg_[j] += vel * kRadToDeg * dt;
      }

      bridge_.sendAction(pos_deg_);
      auto obs = bridge_.getObservation();

      sensor_msgs::msg::JointState js;
      js.header.stamp = this->get_clock()->now();
      js.name = joint_names_;
      js.position.reserve(joint_names_.size());
      for (const auto & j : joint_names_) {
        auto it = obs.find(j + ".pos");
        js.position.push_back(
          it != obs.end() ? it->second * kDegToRad : std::nan(""));
      }
      state_pub_->publish(js);

    } catch (const std::exception & e) {
      RCLCPP_ERROR(get_logger(), "Hardware tick failed, holding position: %s", e.what());
      publishDiag(
        diagnostic_msgs::msg::DiagnosticStatus::ERROR,
        std::string("Hardware tick exception, holding last commanded position: ") + e.what());
      // Deliberately not re-thrown: skip this tick's motion, keep node alive (design doc §10).
    }
  }

  // params
  std::string port_;
  std::string robot_id_;
  std::string venv_site_packages_;
  double control_rate_hz_;
  int joint_cmd_timeout_ms_;
  double overrun_ratio_;
  std::vector<std::string> joint_names_;
  double nominal_dt_;

  // state
  so101_hardware_interface_cpp::SoFollowerBridge bridge_;
  std::map<std::string, double> pos_deg_;
  std::map<std::string, double> latest_velocities_;
  std::optional<rclcpp::Time> last_cmd_stamp_;
  std::chrono::steady_clock::time_point last_tick_time_;
  bool has_last_tick_ = false;

  // ROS I/O
  rclcpp::Subscription<so101_msgs::msg::JointCommand>::SharedPtr cmd_sub_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr state_pub_;
  rclcpp::Publisher<diagnostic_msgs::msg::DiagnosticArray>::SharedPtr diag_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  try {
    auto node = std::make_shared<HardwareInterfaceNode>();
    rclcpp::spin(node);
  } catch (const std::exception & e) {
    RCLCPP_FATAL(rclcpp::get_logger("hardware_interface_node"), "Startup failed: %s", e.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
