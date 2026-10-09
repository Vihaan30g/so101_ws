#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <thread>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>

#include "so101_velocity_control/tcp_line_client.hpp"

namespace
{
constexpr std::array<const char *, 6> kJointNames{
  "shoulder_pan", "shoulder_lift", "elbow_flex",
  "wrist_flex", "wrist_roll", "gripper"};
}

class VelocityCommandBridge : public rclcpp::Node
{
public:
  VelocityCommandBridge()
  : Node("so101_velocity_command_bridge")
  {
    host_ = declare_parameter<std::string>("host", "127.0.0.1");
    port_ = declare_parameter<int>("port", 50012);
    reconnect_interval_ms_ = declare_parameter<int>("reconnect_interval_ms", 1000);
    command_topic_ = declare_parameter<std::string>(
      "command_topic", "/so101/command/velocity");
    state_topic_ = declare_parameter<std::string>(
      "state_topic", "/so101/actual_joint_states");

    command_sub_ = create_subscription<sensor_msgs::msg::JointState>(
      command_topic_, rclcpp::SensorDataQoS(),
      [this](sensor_msgs::msg::JointState::ConstSharedPtr msg) { onCommand(*msg); });
    state_pub_ = create_publisher<sensor_msgs::msg::JointState>(state_topic_, 10);
    reconnect_timer_ = create_wall_timer(
      std::chrono::milliseconds(reconnect_interval_ms_), [this]() { ensureConnected(); });
    retry_command_timer_ = create_wall_timer(
      std::chrono::milliseconds(10), [this]() { sendPendingCommand(); });
    reader_thread_ = std::thread([this]() { readLoop(); });

    RCLCPP_INFO(
      get_logger(), "Velocity bridge listening on %s; LeRobot server target %s:%d",
      command_topic_.c_str(), host_.c_str(), port_);
  }

  ~VelocityCommandBridge() override
  {
    running_ = false;
    client_.close();
    if (reader_thread_.joinable()) {
      reader_thread_.join();
    }
  }

private:
  void ensureConnected()
  {
    if (client_.isConnected()) {
      return;
    }
    std::string error;
    if (client_.connect(host_, port_, error)) {
      RCLCPP_INFO(get_logger(), "Connected to SO-101 velocity server");
    } else {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 2000, "Velocity server unavailable: %s", error.c_str());
    }
  }

  void onCommand(const sensor_msgs::msg::JointState & msg)
  {
    if (msg.name.size() != kJointNames.size() || msg.velocity.size() != kJointNames.size()) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 1000,
        "Velocity command requires six named joints and six velocity values");
      return;
    }

    std::array<double, 6> velocities{};
    std::array<bool, 6> found{};
    for (size_t input = 0; input < msg.name.size(); ++input) {
      size_t output = 0;
      while (output < kJointNames.size() && msg.name[input] != kJointNames[output]) {
        ++output;
      }
      if (output == kJointNames.size() || found[output] || !std::isfinite(msg.velocity[input])) {
        RCLCPP_WARN_THROTTLE(
          get_logger(), *get_clock(), 1000,
          "Velocity command has an unknown/duplicate joint or a non-finite value");
        return;
      }
      velocities[output] = msg.velocity[input];
      found[output] = true;
    }
    if (!std::all_of(found.begin(), found.end(), [](bool value) {return value;})) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000, "Velocity command is incomplete");
      return;
    }

    std::ostringstream line;
    line << std::setprecision(17) << "W1 "
         << std::chrono::duration<double>(
      std::chrono::system_clock::now().time_since_epoch()).count();
    for (const double velocity : velocities) {
      line << ' ' << velocity;
    }
    line << '\n';
    pending_line_ = line.str();
    have_pending_line_ = true;
    sendPendingCommand();
  }

  void sendPendingCommand()
  {
    if (!have_pending_line_ || !client_.isConnected()) {
      return;
    }
    if (client_.send(pending_line_)) {
      have_pending_line_ = false;
    } else {
      client_.close();
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000, "Velocity command send failed");
    }
  }

  void readLoop()
  {
    while (running_) {
      if (!client_.isConnected()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        continue;
      }
      const auto line = client_.readLine();
      if (!line) {
        continue;
      }
      std::istringstream input(*line);
      std::string tag;
      double timestamp;
      std::array<double, 6> positions{};
      int status;
      if (!(input >> tag >> timestamp) || tag != "S1") {
        continue;
      }
      bool valid = true;
      for (double & position : positions) {
        valid = valid && static_cast<bool>(input >> position) && std::isfinite(position);
      }
      if (!valid || !(input >> status)) {
        continue;
      }
      sensor_msgs::msg::JointState state;
      state.header.stamp = now();
      for (const auto * name : kJointNames) {
        state.name.emplace_back(name);
      }
      state.position.assign(positions.begin(), positions.end());
      state_pub_->publish(state);
    }
  }

  std::string host_;
  int port_{50012};
  int reconnect_interval_ms_{1000};
  std::string command_topic_;
  std::string state_topic_;
  std::string pending_line_;
  bool have_pending_line_{false};
  TcpLineClient client_;
  std::atomic<bool> running_{true};
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr command_sub_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr state_pub_;
  rclcpp::TimerBase::SharedPtr reconnect_timer_;
  rclcpp::TimerBase::SharedPtr retry_command_timer_;
  std::thread reader_thread_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VelocityCommandBridge>());
  rclcpp::shutdown();
  return 0;
}
