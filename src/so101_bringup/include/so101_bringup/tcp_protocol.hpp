#pragma once
// tcp_protocol.hpp
//
// Minimal, dependency-free wire protocol between:
//   - tcp_bridge_node (C++ / rclcpp, runs under ROS 2 Humble's Python 3.10 env
//     for the ROS side, but this file itself is pure C++ with zero ROS
//     message types so it can be unit-tested standalone)
//   - lerobot_hardware_server.py (Python 3.12, lerobot conda env)
//
// Rationale for hand-rolled text lines instead of JSON: at 100-200 Hz the
// payload is fixed-shape (always exactly 6 joints), so a generic JSON
// parser is unneeded overhead and an extra dependency (nlohmann/json is
// not guaranteed present in a bare ROS 2 Humble install). Plain
// whitespace-separated ASCII is trivial to parse on both ends, trivial to
// eyeball with `nc localhost <port>` while debugging, and the framing is
// simply "one message per newline" over a TCP stream (TCP already
// guarantees in-order, complete delivery on localhost, so a length prefix
// isn't necessary -- just buffer until you see '\n').
//
// Wire format (all ASCII, fields space-separated, line ends with '\n'):
//
//   Bridge -> Server (command), one line per control tick:
//     V1 <unix_time_sec:double> <v0> <v1> <v2> <v3> <v4> <v5> <control_mode:int>
//     where v0..v5 are commanded joint velocities in rad/s, in the fixed
//     joint order [shoulder_pan, shoulder_lift, elbow_flex, wrist_flex,
//     wrist_roll, gripper], and control_mode is
//     so101_msgs::msg::JointCommand::NORMAL(0) or FROZEN_TRANSLATION(1).
//
//   Server -> Bridge (state), one line per hardware tick:
//     S1 <unix_time_sec:double> <p0> <p1> <p2> <p3> <p4> <p5> <status:int>
//     where p0..p5 are measured joint positions in RADIANS, same fixed
//     joint order as above, and status is 0=OK, 1=WARN, 2=ERROR (mirrors
//     diagnostic_msgs::msg::DiagnosticStatus levels).
//
// The "V1"/"S1" tags are a trivial version marker so the protocol can grow
// a V2/S2 later without ambiguity.

#include <array>
#include <cstdio>
#include <optional>
#include <sstream>
#include <string>

namespace so101_control
{

constexpr int kNumJoints = 6;

struct JointVelocityCommandLine
{
  double unix_time{0.0};
  std::array<double, kNumJoints> velocities{};
  int control_mode{0};
};

struct JointStateLine
{
  double unix_time{0.0};
  std::array<double, kNumJoints> positions{};
  int status{0};
};

// Formats a command line ending in '\n', ready to send() as-is.
inline std::string encodeCommand(const JointVelocityCommandLine & cmd)
{
  std::ostringstream oss;
  oss << "V1 " << cmd.unix_time;
  for (double v : cmd.velocities) {
    oss << ' ' << v;
  }
  oss << ' ' << cmd.control_mode << '\n';
  return oss.str();
}

// Parses one line (without trailing newline, but trailing newline is
// tolerated too) of the form "S1 <t> <p0..p5> <status>". Returns
// std::nullopt on any malformed line rather than throwing, since a single
// corrupt line on a live control link should never crash the bridge --
// the caller should just skip it and keep reading.
inline std::optional<JointStateLine> decodeState(const std::string & line)
{
  std::istringstream iss(line);
  std::string tag;
  iss >> tag;
  if (tag != "S1") {
    return std::nullopt;
  }
  JointStateLine s;
  if (!(iss >> s.unix_time)) {
    return std::nullopt;
  }
  for (int i = 0; i < kNumJoints; ++i) {
    if (!(iss >> s.positions[static_cast<size_t>(i)])) {
      return std::nullopt;
    }
  }
  if (!(iss >> s.status)) {
    return std::nullopt;
  }
  return s;
}

}  // namespace so101_control
