#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <so101_msgs/msg/joint_command.hpp>

#include "tcp_client.hpp"
#include "tcp_protocol.hpp"

class TcpBridgeNode : public rclcpp::Node
{
public:
  TcpBridgeNode()
  : Node("tcp_bridge_node")
  {
    host_ = declare_parameter<std::string>("host", "127.0.0.1");
    port_ = declare_parameter<int>("port", 50010);
    reconnect_interval_ms_ = declare_parameter<int>("reconnect_interval_ms", 1000);
    command_rate_hz_ = declare_parameter<double>("command_rate_hz", 100.0);
    state_publish_hz_ = declare_parameter<double>("state_publish_hz", 50.0);
    // NOTE: staleness/timeout handling lives upstream (cartesian_controller_node's
    // per-input timeouts, safety_gate_node's command/state-age checks, and
    // lerobot_server.py's own watchdog) -- this bridge is a thin, dumb pipe and
    // deliberately does not duplicate that logic here.

    joint_order_ = {"shoulder_pan", "shoulder_lift", "elbow_flex", "wrist_flex", "wrist_roll", "gripper"};

    cmd_sub_ = create_subscription<so101_msgs::msg::JointCommand>(
      "/joint_cmd_safe", rclcpp::SensorDataQoS(),
      [this](so101_msgs::msg::JointCommand::SharedPtr msg) {
        std::lock_guard<std::mutex> lock(cmd_mutex_);
        pending_cmd_ = *msg;
        have_pending_cmd_ = true;
      });

    joint_state_pub_ = create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);

    send_timer_ = create_wall_timer(
      std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(1.0 / command_rate_hz_)),
      [this]() { sendPendingCommand(); });

    reconnect_timer_ = create_wall_timer(
      std::chrono::milliseconds(reconnect_interval_ms_), [this]() { ensureConnected(); });

    read_thread_ = std::thread([this]() { readLoop(); });

    RCLCPP_INFO(get_logger(), "TCP bridge ready: %s:%d", host_.c_str(), port_);
  }

  ~TcpBridgeNode() override
  {
    running_ = false;
    if (read_thread_.joinable()) {
      read_thread_.join();
    }
    client_.close();
  }

private:
  void ensureConnected()
  {
    if (client_.isConnected()) {
      return;
    }

    std::string err;
    if (client_.connect(host_, port_, err)) {
      RCLCPP_INFO(get_logger(), "Connected to LeRobot TCP server at %s:%d", host_.c_str(), port_);
    } else {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 2000, "Unable to connect to LeRobot server: %s", err.c_str());
    }
  }

  void sendPendingCommand()
  {
    if (!client_.isConnected()) {
      return;
    }

    std::lock_guard<std::mutex> lock(cmd_mutex_);
    if (!have_pending_cmd_) {
      return;
    }

    so101_msgs::msg::JointCommand cmd = pending_cmd_;
    have_pending_cmd_ = false;

    if (cmd.joint_names.size() != 6 || cmd.velocities.size() != 6) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000, "Unsafe command dropped by bridge: wrong size");
      return;
    }

    std::array<double, so101_control::kNumJoints> velocities{};
    for (size_t i = 0; i < joint_order_.size(); ++i) {
      const std::string & name = joint_order_[i];
      auto it = std::find(cmd.joint_names.begin(), cmd.joint_names.end(), name);
      if (it == cmd.joint_names.end()) {
        velocities[i] = 0.0;
        continue;
      }
      const size_t idx = static_cast<size_t>(std::distance(cmd.joint_names.begin(), it));
      if (idx < cmd.velocities.size()) {
        velocities[i] = cmd.velocities[idx];
      } else {
        velocities[i] = 0.0;
      }
    }

    so101_control::JointVelocityCommandLine line;
    line.unix_time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::steady_clock::now().time_since_epoch()).count()) / 1e6;
    line.velocities = velocities;
    line.control_mode = static_cast<int>(cmd.control_mode);

    const std::string encoded = so101_control::encodeCommand(line);
    if (!client_.send(encoded)) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000, "Send failed, closing socket");
      client_.close();
    }
  }

  void readLoop()
  {
    while (running_) {
      if (!client_.isConnected()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        continue;
      }

      const auto maybe_line = client_.readLine();
      if (!maybe_line.has_value()) {
        if (!client_.isConnected()) {
          RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000, "LeRobot server disconnected");
        }
        continue;
      }

      const auto decoded = so101_control::decodeState(*maybe_line);
      if (!decoded.has_value()) {
        continue;
      }

      sensor_msgs::msg::JointState state;
      state.header.stamp = now();
      state.name = joint_order_;
      state.position.resize(6);
      state.velocity.resize(6, 0.0);
      state.effort.resize(6, 0.0);
      for (size_t i = 0; i < joint_order_.size(); ++i) {
        state.position[i] = decoded->positions[i];
      }
      joint_state_pub_->publish(state);
    }
  }

  std::string host_;
  int port_{50010};
  int reconnect_interval_ms_{1000};
  double command_rate_hz_{100.0};
  double state_publish_hz_{50.0};

  std::vector<std::string> joint_order_;
  std::mutex cmd_mutex_;
  so101_msgs::msg::JointCommand pending_cmd_{};
  bool have_pending_cmd_{false};

  so101_control::TcpClient client_;
  std::atomic<bool> running_{true};
  rclcpp::Subscription<so101_msgs::msg::JointCommand>::SharedPtr cmd_sub_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_state_pub_;
  rclcpp::TimerBase::SharedPtr send_timer_;
  rclcpp::TimerBase::SharedPtr reconnect_timer_;
  std::thread read_thread_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TcpBridgeNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
