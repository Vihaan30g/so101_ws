#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <so101_msgs/msg/controller_status.hpp>
#include <so101_msgs/msg/joint_command.hpp>

#include "so101_kinematics/so101_kinematics.hpp"

namespace {

bool isFiniteVec(const std::vector<double> & values)
{
  for (double v : values) {
    if (!std::isfinite(v)) {
      return false;
    }
  }
  return true;
}

std::vector<std::string> defaultJointOrder()
{
  return {"shoulder_pan", "shoulder_lift", "elbow_flex", "wrist_flex", "wrist_roll", "gripper"};
}

}  // namespace

class SafetyGateNode : public rclcpp::Node
{
public:
  SafetyGateNode()
  : Node("safety_gate_node")
  {
    urdf_path_ = declare_parameter<std::string>("urdf_path", "");
    end_effector_frame_ = declare_parameter<std::string>("end_effector_frame", "gripper_frame_link");
    translational_joints_ = declare_parameter<std::vector<std::string>>("translational_joints", {"shoulder_pan", "shoulder_lift", "elbow_flex"});
    wrist_joints_ = declare_parameter<std::vector<std::string>>("wrist_joints", {"wrist_flex", "wrist_roll"});
    gripper_joint_ = declare_parameter<std::string>("gripper_joint", "gripper");
    joint_timeout_ms_ = declare_parameter<int>("joint_cmd_timeout_ms", 100);
    manipulability_floor_ = declare_parameter<double>("manipulability_hard_floor", 0.005);
    measured_position_tolerance_rad_ = declare_parameter<double>(
      "measured_position_tolerance_rad", 0.02);

    if (urdf_path_.empty()) {
      RCLCPP_FATAL(get_logger(), "urdf_path parameter is empty; refusing to start safety gate.");
      throw std::runtime_error("safety_gate_node: urdf_path is required");
    }

    so101_kinematics::KinematicsConfig cfg;
    cfg.urdf_path = urdf_path_;
    cfg.end_effector_frame = end_effector_frame_;
    cfg.translational_joints = translational_joints_;
    cfg.wrist_joints = wrist_joints_;
    cfg.dls_lambda0 = 0.05;
    cfg.manipulability_threshold = manipulability_floor_;
    kin_ = std::make_unique<so101_kinematics::So101Kinematics>(cfg);

    joint_names_ = translational_joints_;
    joint_names_.insert(joint_names_.end(), wrist_joints_.begin(), wrist_joints_.end());
    joint_names_.push_back(gripper_joint_);
    if (joint_names_.size() != 6) {
      throw std::runtime_error("safety_gate_node: expected 6 joints in the configured ordering");
    }

    joint_cmd_sub_ = create_subscription<so101_msgs::msg::JointCommand>(
      "/joint_cmd", rclcpp::SensorDataQoS(),
      [this](so101_msgs::msg::JointCommand::SharedPtr msg) { handleCommand(msg); });

    joint_state_sub_ = create_subscription<sensor_msgs::msg::JointState>(
      "/joint_states", rclcpp::SensorDataQoS(),
      [this](sensor_msgs::msg::JointState::SharedPtr msg) {
        std::lock_guard<std::mutex> lock(state_mutex_);
        latest_joint_state_ = *msg;
        last_joint_state_stamp_ = now();
        have_state_ = true;
        for (size_t i = 0; i < msg->name.size() && i < msg->position.size(); ++i) {
          latest_positions_[msg->name[i]] = msg->position[i];
        }
      });

    joint_cmd_safe_pub_ = create_publisher<so101_msgs::msg::JointCommand>("/joint_cmd_safe", 10);
    
    // FIX: Changed from <so101_msgs::msg::JointCommand> to <so101_msgs::msg::ControllerStatus>
    safe_status_pub_ = create_publisher<so101_msgs::msg::ControllerStatus>("/joint_cmd_safe_status", 10);

    RCLCPP_INFO(get_logger(), "Safety gate ready. Required joint order: %s, %s, %s, %s, %s, %s",
      joint_names_[0].c_str(), joint_names_[1].c_str(), joint_names_[2].c_str(),
      joint_names_[3].c_str(), joint_names_[4].c_str(), joint_names_[5].c_str());
  }

private:
  void handleCommand(const so101_msgs::msg::JointCommand::SharedPtr & msg)
  {
    so101_msgs::msg::JointCommand safe_cmd;
    safe_cmd.header.stamp = now();
    safe_cmd.control_mode = so101_msgs::msg::JointCommand::NORMAL;
    safe_cmd.joint_names = joint_names_;
    safe_cmd.velocities.assign(6, 0.0);

    if (!msg) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000, "Received null JointCommand; sending zero");
      publishStatus(false, "null command");
      publishSafe(safe_cmd);
      return;
    }

    std::string error;
    if (!validateCommand(*msg, safe_cmd, error)) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000, "Rejected unsafe command: %s", error.c_str());
      publishStatus(false, error);
      publishSafe(safe_cmd);
      return;
    }

    publishStatus(true, "");
    publishSafe(safe_cmd);
  }

  bool validateCommand(const so101_msgs::msg::JointCommand & msg, so101_msgs::msg::JointCommand & safe_cmd, std::string & error)
  {
    if (msg.joint_names.size() != 6 || msg.velocities.size() != 6) {
      error = "JointCommand must have exactly 6 names and 6 velocity entries";
      return false;
    }

    if (msg.control_mode != so101_msgs::msg::JointCommand::NORMAL &&
        msg.control_mode != so101_msgs::msg::JointCommand::FROZEN_TRANSLATION) {
      error = "control_mode is not valid";
      return false;
    }

    if (!isFiniteVec(msg.velocities)) {
      error = "command contains NaN or Inf values";
      return false;
    }

    if (msg.joint_names != joint_names_) {
      error = "joint_names ordering does not match required SO-101 order";
      return false;
    }

    if (msg.header.stamp.sec == 0 && msg.header.stamp.nanosec == 0) {
      error = "header stamp is uninitialized";
      return false;
    }

    const auto now_time = now();
    const auto age = now_time - msg.header.stamp;
    if (age > rclcpp::Duration::from_nanoseconds(1'000'000LL * joint_timeout_ms_)) {
      error = "JointCommand is stale; stopping motion";
      return false;
    }

    std::lock_guard<std::mutex> lock(state_mutex_);
    if (have_state_) {
      const auto age_state = now_time - last_joint_state_stamp_;
      if (age_state > rclcpp::Duration::from_nanoseconds(1'000'000LL * joint_timeout_ms_)) {
        error = "joint_states are stale; hardware state is not fresh enough for safe command evaluation";
        return false;
      }
    }

    Eigen::VectorXd q = Eigen::VectorXd::Zero(kin_->nq());
    for (const auto & jname : kin_->jointOrder()) {
      auto it = latest_positions_.find(jname);
      if (it != latest_positions_.end()) {
        if (!std::isfinite(it->second)) {
          error = "current joint position is not finite";
          return false;
        }
        if (!kin_->model().existJointName(jname)) {
          continue;
        }
        const auto jid = kin_->model().getJointId(jname);
        const int idx_q = kin_->model().joints[jid].idx_q();
        if (idx_q >= 0 && idx_q < q.size()) {
          q[idx_q] = it->second;
        }
      }
    }

    const auto & lower = kin_->lowerPositionLimits();
    const auto & upper = kin_->upperPositionLimits();
    for (int i = 0; i < static_cast<int>(joint_names_.size()); ++i) {
      const std::string & name = joint_names_[static_cast<size_t>(i)];
      if (!kin_->model().existJointName(name)) {
        error = "joint name not found in URDF: " + name;
        return false;
      }
      const auto jid = kin_->model().getJointId(name);
      const int idx_q = kin_->model().joints[jid].idx_q();
      if (idx_q >= 0 && idx_q < q.size()) {
        const double qv = q[idx_q];
        if (qv < lower[idx_q] - measured_position_tolerance_rad_ ||
            qv > upper[idx_q] + measured_position_tolerance_rad_) {
          error = "current joint position is outside its URDF limits";
          return false;
        }
      }
    }

    const auto & vel_limits = kin_->velocityLimits();
    for (size_t i = 0; i < msg.velocities.size(); ++i) {
      const double v = msg.velocities[i];
      if (i < vel_limits.size()) {
        const double max_vel = std::abs(vel_limits[static_cast<Eigen::Index>(i)]);
        if (std::abs(v) > max_vel + 1e-9) {
          error = "velocity exceeds URDF limit on joint " + joint_names_[i];
          return false;
        }
      }
    }

    if (msg.control_mode == so101_msgs::msg::JointCommand::FROZEN_TRANSLATION) {
      safe_cmd = msg;
      safe_cmd.velocities.assign(6, 0.0);
      safe_cmd.velocities[0] = msg.velocities[0];
      safe_cmd.velocities[1] = msg.velocities[1];
      safe_cmd.velocities[2] = msg.velocities[2];
      safe_cmd.velocities[3] = msg.velocities[3];
      safe_cmd.velocities[4] = msg.velocities[4];
      safe_cmd.velocities[5] = msg.velocities[5];
      return true;
    }

    safe_cmd = msg;
    return true;
  }

  void publishStatus(bool healthy, const std::string & message)
  {
    so101_msgs::msg::ControllerStatus status;
    status.header.stamp = now();
    status.in_singularity = false;
    status.manipulability_index = 1.0;
    status.near_joint_limit = !healthy;
    status.self_collision_detected = false;
    status.last_error = message;
    safe_status_pub_->publish(status);
  }

  void publishSafe(const so101_msgs::msg::JointCommand & safe_cmd)
  {
    so101_msgs::msg::JointCommand out = safe_cmd;
    out.header.stamp = now();
    out.joint_names = joint_names_;
    if (out.velocities.size() != 6) {
      out.velocities.assign(6, 0.0);
    }
    for (size_t i = 0; i < out.velocities.size(); ++i) {
      if (!std::isfinite(out.velocities[i])) {
        out.velocities[i] = 0.0;
      }
    }
    joint_cmd_safe_pub_->publish(out);
  }

  std::string urdf_path_;
  std::string end_effector_frame_;
  std::string gripper_joint_;
  std::vector<std::string> translational_joints_;
  std::vector<std::string> wrist_joints_;
  std::vector<std::string> joint_names_;
  int joint_timeout_ms_{100};
  double manipulability_floor_{0.005};
  double measured_position_tolerance_rad_{0.02};

  std::unique_ptr<so101_kinematics::So101Kinematics> kin_;

  rclcpp::Subscription<so101_msgs::msg::JointCommand>::SharedPtr joint_cmd_sub_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;
  rclcpp::Publisher<so101_msgs::msg::JointCommand>::SharedPtr joint_cmd_safe_pub_;
  rclcpp::Publisher<so101_msgs::msg::ControllerStatus>::SharedPtr safe_status_pub_;

  std::mutex state_mutex_;
  sensor_msgs::msg::JointState latest_joint_state_;
  rclcpp::Time last_joint_state_stamp_;
  bool have_state_{false};
  std::unordered_map<std::string, double> latest_positions_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  try {
    auto node = std::make_shared<SafetyGateNode>();
    rclcpp::spin(node);
  } catch (const std::exception & e) {
    RCLCPP_FATAL(rclcpp::get_logger("safety_gate_node"), "Fatal: %s", e.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}