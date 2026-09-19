// cartesian_controller_node.cpp
//
// Implements the "cartesian_controller_node" role from robot_params.yaml
// and the so101_msgs pipeline comments:
//
//   CartesianCommand -> cartesian_controller_node -> JointCommand
//
// Inputs:
//   /cartesian_cmd   (so101_msgs/CartesianCommand)  -- translation target,
//                     "velocity" or "pose" mode, from whatever input node
//                     is active (keyboard today).
//   /wrist_cmd       (so101_msgs/JointVelocityCommand) -- direct teleop
//                     velocities for wrist_flex/wrist_roll (not IK
//                     controlled, per robot_params.yaml).
//   /gripper_cmd     (std_msgs/Float64) -- direct teleop velocity for the
//                     gripper joint (the 6th joint the original design
//                     doc didn't account for -- see robot_params.yaml).
//   /joint_states    (sensor_msgs/JointState) -- current measured joint
//                     positions, needed to evaluate the Jacobian at the
//                     current configuration. Published by tcp_bridge_node.
//
// Output:
//   /joint_cmd           (so101_msgs/JointCommand) -- UNSAFE full 6-joint
//                         velocity command. safety_gate_node consumes
//                         this and is the only thing allowed to publish
//                         to /joint_cmd_safe.
//   /controller_status   (so101_msgs/ControllerStatus) -- manipulability /
//                         singularity / joint-limit telemetry.
//
// Any input that goes stale (no message within its configured timeout,
// or CartesianCommand.valid == false) is treated as zero velocity on
// that input -- "user let go -> stop", per the design doc.

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/float64.hpp>

#include <so101_msgs/msg/cartesian_command.hpp>
#include <so101_msgs/msg/controller_status.hpp>
#include <so101_msgs/msg/joint_command.hpp>
#include <so101_msgs/msg/joint_velocity_command.hpp>

#include "so101_kinematics/so101_kinematics.hpp"

using namespace std::chrono_literals;

namespace
{
Eigen::Vector3d clampNorm(const Eigen::Vector3d & v, double max_norm)
{
  const double n = v.norm();
  if (n > max_norm && n > 1e-9) {
    return v * (max_norm / n);
  }
  return v;
}
}  // namespace

class CartesianControllerNode : public rclcpp::Node
{
public:
  CartesianControllerNode()
  : Node("cartesian_controller_node")
  {
    // --- Parameters (defaults mirror robot_params.yaml) ---
    urdf_path_ = declare_parameter<std::string>("urdf_path", "");
    end_effector_frame_ = declare_parameter<std::string>("end_effector_frame", "gripper_frame_link");
    translational_joints_ = declare_parameter<std::vector<std::string>>(
      "translational_joints", {"shoulder_pan", "shoulder_lift", "elbow_flex"});
    wrist_joints_ = declare_parameter<std::vector<std::string>>(
      "wrist_joints", {"wrist_flex", "wrist_roll"});
    gripper_joint_ = declare_parameter<std::string>("gripper_joint", "gripper");

    control_rate_hz_ = declare_parameter<double>("control_rate_hz", 200.0);
    cartesian_cmd_timeout_ms_ = declare_parameter<int>("cartesian_cmd_timeout_ms", 200);
    wrist_cmd_timeout_ms_ = declare_parameter<int>("wrist_cmd_timeout_ms", 200);
    gripper_cmd_timeout_ms_ = declare_parameter<int>("gripper_cmd_timeout_ms", 200);

    dls_lambda0_ = declare_parameter<double>("dls_lambda0", 0.05);
    manipulability_threshold_ = declare_parameter<double>("manipulability_threshold", 0.02);

    pose_mode_kp_ = declare_parameter<double>("pose_mode_kp", 1.5);
    pose_mode_max_speed_ = declare_parameter<double>("pose_mode_max_speed", 0.15);
    joint_limit_margin_rad_ = declare_parameter<double>("joint_limit_margin_rad", 0.05);

    if (urdf_path_.empty()) {
      RCLCPP_FATAL(get_logger(), "urdf_path parameter is empty -- cannot start.");
      throw std::runtime_error("cartesian_controller_node: urdf_path is required");
    }

    so101_kinematics::KinematicsConfig cfg;
    cfg.urdf_path = urdf_path_;
    cfg.end_effector_frame = end_effector_frame_;
    cfg.translational_joints = translational_joints_;
    cfg.wrist_joints = wrist_joints_;
    cfg.dls_lambda0 = dls_lambda0_;
    cfg.manipulability_threshold = manipulability_threshold_;

    try {
      kin_ = std::make_unique<so101_kinematics::So101Kinematics>(cfg);
    } catch (const std::exception & e) {
      RCLCPP_FATAL(get_logger(), "Failed to build kinematics model: %s", e.what());
      throw;
    }
    RCLCPP_INFO(
      get_logger(), "Loaded kinematics: nq=%d nv=%d ee_frame=%s", kin_->nq(), kin_->nv(),
      end_effector_frame_.c_str());

    joint_names_out_ = translational_joints_;
    joint_names_out_.insert(joint_names_out_.end(), wrist_joints_.begin(), wrist_joints_.end());
    joint_names_out_.push_back(gripper_joint_);

    // --- Subscriptions ---
    cart_sub_ = create_subscription<so101_msgs::msg::CartesianCommand>(
      "/cartesian_cmd", rclcpp::SensorDataQoS(),
      [this](so101_msgs::msg::CartesianCommand::SharedPtr msg) {
        std::lock_guard<std::mutex> lock(cart_mutex_);
        last_cart_ = *msg;
        last_cart_stamp_ = now();
        have_cart_ = true;
      });

    wrist_sub_ = create_subscription<so101_msgs::msg::JointVelocityCommand>(
      "/wrist_cmd", rclcpp::SensorDataQoS(),
      [this](so101_msgs::msg::JointVelocityCommand::SharedPtr msg) {
        std::lock_guard<std::mutex> lock(wrist_mutex_);
        last_wrist_ = *msg;
        last_wrist_stamp_ = now();
        have_wrist_ = true;
      });

    gripper_sub_ = create_subscription<std_msgs::msg::Float64>(
      "/gripper_cmd", rclcpp::SensorDataQoS(),
      [this](std_msgs::msg::Float64::SharedPtr msg) {
        std::lock_guard<std::mutex> lock(gripper_mutex_);
        last_gripper_vel_ = msg->data;
        last_gripper_stamp_ = now();
        have_gripper_ = true;
      });

    joint_state_sub_ = create_subscription<sensor_msgs::msg::JointState>(
      "/joint_states", rclcpp::SensorDataQoS(),
      [this](sensor_msgs::msg::JointState::SharedPtr msg) {
        std::lock_guard<std::mutex> lock(joint_state_mutex_);
        for (size_t i = 0; i < msg->name.size() && i < msg->position.size(); ++i) {
          latest_positions_[msg->name[i]] = msg->position[i];
        }
        have_joint_state_ = true;
      });

    // --- Publishers ---
    joint_cmd_pub_ = create_publisher<so101_msgs::msg::JointCommand>("/joint_cmd", 10);
    status_pub_ = create_publisher<so101_msgs::msg::ControllerStatus>("/controller_status", 10);

    const auto period = std::chrono::duration<double>(1.0 / control_rate_hz_);
    timer_ = create_wall_timer(
      std::chrono::duration_cast<std::chrono::nanoseconds>(period), [this] { controlTick(); });

    RCLCPP_INFO(get_logger(), "cartesian_controller_node ready at %.1f Hz", control_rate_hz_);
  }

private:
  bool stale(const rclcpp::Time & stamp, int timeout_ms)
  {
    return (now() - stamp) > rclcpp::Duration(std::chrono::milliseconds(timeout_ms));
  }

  Eigen::VectorXd buildQ()
  {
    Eigen::VectorXd q = Eigen::VectorXd::Zero(kin_->nq());
    std::lock_guard<std::mutex> lock(joint_state_mutex_);
    for (const auto & jname : kin_->jointOrder()) {
      auto it = latest_positions_.find(jname);
      if (it == latest_positions_.end()) {
        continue;
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
    return q;
  }

  void controlTick()
  {
    if (!have_joint_state_) {
      RCLCPP_WARN_THROTTLE(
        get_logger(), *get_clock(), 2000,
        "No /joint_states received yet -- cannot run IK, holding.");
      publishZero(so101_msgs::msg::JointCommand::NORMAL);
      return;
    }

    const Eigen::VectorXd q = buildQ();

    // --- Translational velocity command (Cartesian, IK-controlled) ---
    Eigen::Vector3d v_cmd = Eigen::Vector3d::Zero();
    {
      std::lock_guard<std::mutex> lock(cart_mutex_);
      const bool cart_stale = !have_cart_ || stale(last_cart_stamp_, cartesian_cmd_timeout_ms_);
      if (!cart_stale && last_cart_.valid) {
        if (last_cart_.mode == "velocity") {
          v_cmd = Eigen::Vector3d(
            last_cart_.velocity.linear.x, last_cart_.velocity.linear.y, last_cart_.velocity.linear.z);
        } else if (last_cart_.mode == "pose") {
          const Eigen::Vector3d ee_pos = kin_->computeEndEffectorPosition(q);
          const Eigen::Vector3d target(
            last_cart_.target_pose.position.x, last_cart_.target_pose.position.y,
            last_cart_.target_pose.position.z);
          v_cmd = clampNorm(pose_mode_kp_ * (target - ee_pos), pose_mode_max_speed_);
        } else {
          RCLCPP_WARN_THROTTLE(
            get_logger(), *get_clock(), 2000, "Unknown CartesianCommand.mode '%s', holding.",
            last_cart_.mode.c_str());
        }
      }
    }

    so101_kinematics::IKResult ik = kin_->solveDLS(q, v_cmd);

    // FIX: nearJointLimit() previously only fed a status flag on
    // /controller_status that nothing consumed to actually stop motion.
    // Clamp each translational joint's own velocity component to zero if
    // it's within joint_limit_margin_rad_ of its limit AND still moving
    // further toward that limit. This is a soft, per-joint clamp applied
    // AFTER the DLS solve -- it slightly distorts pure Cartesian tracking
    // right at the boundary, which is the correct trade to make: safety
    // over precision at the edge of travel. This is defense-in-depth on
    // top of (not a replacement for) POSITION-mode firmware limits on the
    // hardware side and safety_gate_node's reactive check downstream.
    for (size_t i = 0; i < translational_joints_.size() &&
                        static_cast<Eigen::Index>(i) < ik.joint_velocities.size(); ++i) {
      ik.joint_velocities[static_cast<Eigen::Index>(i)] =
        clampTowardLimit(translational_joints_[i], q, ik.joint_velocities[static_cast<Eigen::Index>(i)]);
    }

    // --- Wrist velocities (direct teleop passthrough) ---
    std::vector<double> wrist_vel(wrist_joints_.size(), 0.0);
    {
      std::lock_guard<std::mutex> lock(wrist_mutex_);
      const bool wrist_stale = !have_wrist_ || stale(last_wrist_stamp_, wrist_cmd_timeout_ms_);
      if (!wrist_stale) {
        for (size_t i = 0; i < last_wrist_.joint_names.size() && i < last_wrist_.velocities.size(); ++i) {
          auto it = std::find(wrist_joints_.begin(), wrist_joints_.end(), last_wrist_.joint_names[i]);
          if (it != wrist_joints_.end()) {
            wrist_vel[static_cast<size_t>(std::distance(wrist_joints_.begin(), it))] =
              last_wrist_.velocities[i];
          }
        }
      }
    }
    for (size_t i = 0; i < wrist_joints_.size(); ++i) {
      wrist_vel[i] = clampTowardLimit(wrist_joints_[i], q, wrist_vel[i]);
    }

    // --- Gripper velocity (direct teleop passthrough) ---
    double gripper_vel = 0.0;
    {
      std::lock_guard<std::mutex> lock(gripper_mutex_);
      const bool gripper_stale = !have_gripper_ || stale(last_gripper_stamp_, gripper_cmd_timeout_ms_);
      if (!gripper_stale) {
        gripper_vel = last_gripper_vel_;
      }
    }

    // --- Assemble and publish JointCommand ---
    so101_msgs::msg::JointCommand out;
    out.header.stamp = now();
    out.joint_names = joint_names_out_;
    out.velocities.reserve(joint_names_out_.size());
    for (Eigen::Index i = 0; i < ik.joint_velocities.size(); ++i) {
      out.velocities.push_back(ik.joint_velocities[i]);
    }
    for (double v : wrist_vel) {
      out.velocities.push_back(v);
    }
    out.velocities.push_back(gripper_vel);
    out.control_mode = so101_msgs::msg::JointCommand::NORMAL;
    joint_cmd_pub_->publish(out);

    // --- Status telemetry ---
    so101_msgs::msg::ControllerStatus status;
    status.header.stamp = out.header.stamp;
    status.in_singularity = ik.near_singularity;
    status.manipulability_index = ik.manipulability;
    status.near_joint_limit = nearJointLimit(q);
    status.self_collision_detected = kin_->checkSelfCollision(q);  // stub, always false today
    status.last_error = "";
    status_pub_->publish(status);
  }

  // Zero `vel` if joint `name` is within joint_limit_margin_rad_ of its
  // position limit and `vel`'s sign would drive it further past that
  // limit. Joints with no finite limit (e.g. a continuous wrist_roll) are
  // left untouched -- clampTowardLimit only acts on joints that actually
  // have a bounded travel range in the URDF.
  double clampTowardLimit(const std::string & name, const Eigen::VectorXd & q, double vel)
  {
    if (!kin_->model().existJointName(name)) {
      return vel;
    }
    const auto jid = kin_->model().getJointId(name);
    const int idx_q = kin_->model().joints[jid].idx_q();
    if (idx_q < 0 || idx_q >= q.size()) {
      return vel;
    }
    const auto & lower = kin_->lowerPositionLimits();
    const auto & upper = kin_->upperPositionLimits();
    if (idx_q >= static_cast<int>(lower.size()) || idx_q >= static_cast<int>(upper.size())) {
      return vel;
    }
    if (!std::isfinite(lower[idx_q]) || !std::isfinite(upper[idx_q])) {
      return vel;  // continuous joint (e.g. wrist_roll) -- no limit to enforce
    }
    const double val = q[idx_q];
    if (val > upper[idx_q] - joint_limit_margin_rad_ && vel > 0.0) {
      return 0.0;
    }
    if (val < lower[idx_q] + joint_limit_margin_rad_ && vel < 0.0) {
      return 0.0;
    }
    return vel;
  }

  bool nearJointLimit(const Eigen::VectorXd & q)
  {
    const auto & lower = kin_->lowerPositionLimits();
    const auto & upper = kin_->upperPositionLimits();
    for (const auto & jname : translational_joints_) {
      if (!kin_->model().existJointName(jname)) {
        continue;
      }
      const auto jid = kin_->model().getJointId(jname);
      const int idx_q = kin_->model().joints[jid].idx_q();
      if (idx_q < 0 || idx_q >= q.size()) {
        continue;
      }
      const double val = q[idx_q];
      if (val < lower[idx_q] + joint_limit_margin_rad_ || val > upper[idx_q] - joint_limit_margin_rad_) {
        return true;
      }
    }
    return false;
  }

  void publishZero(uint8_t control_mode)
  {
    so101_msgs::msg::JointCommand out;
    out.header.stamp = now();
    out.joint_names = joint_names_out_;
    out.velocities.assign(joint_names_out_.size(), 0.0);
    out.control_mode = control_mode;
    joint_cmd_pub_->publish(out);
  }

  // Parameters
  std::string urdf_path_, end_effector_frame_, gripper_joint_;
  std::vector<std::string> translational_joints_, wrist_joints_, joint_names_out_;
  double control_rate_hz_, dls_lambda0_, manipulability_threshold_;
  int cartesian_cmd_timeout_ms_, wrist_cmd_timeout_ms_, gripper_cmd_timeout_ms_;
  double pose_mode_kp_, pose_mode_max_speed_, joint_limit_margin_rad_;

  std::unique_ptr<so101_kinematics::So101Kinematics> kin_;

  // Cached inputs
  std::mutex cart_mutex_;
  so101_msgs::msg::CartesianCommand last_cart_;
  rclcpp::Time last_cart_stamp_;
  bool have_cart_{false};

  std::mutex wrist_mutex_;
  so101_msgs::msg::JointVelocityCommand last_wrist_;
  rclcpp::Time last_wrist_stamp_;
  bool have_wrist_{false};

  std::mutex gripper_mutex_;
  double last_gripper_vel_{0.0};
  rclcpp::Time last_gripper_stamp_;
  bool have_gripper_{false};

  std::mutex joint_state_mutex_;
  std::unordered_map<std::string, double> latest_positions_;
  bool have_joint_state_{false};

  // ROS interfaces
  rclcpp::Subscription<so101_msgs::msg::CartesianCommand>::SharedPtr cart_sub_;
  rclcpp::Subscription<so101_msgs::msg::JointVelocityCommand>::SharedPtr wrist_sub_;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr gripper_sub_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;
  rclcpp::Publisher<so101_msgs::msg::JointCommand>::SharedPtr joint_cmd_pub_;
  rclcpp::Publisher<so101_msgs::msg::ControllerStatus>::SharedPtr status_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  try {
    auto node = std::make_shared<CartesianControllerNode>();
    rclcpp::spin(node);
  } catch (const std::exception & e) {
    RCLCPP_FATAL(rclcpp::get_logger("cartesian_controller_node"), "Fatal: %s", e.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
