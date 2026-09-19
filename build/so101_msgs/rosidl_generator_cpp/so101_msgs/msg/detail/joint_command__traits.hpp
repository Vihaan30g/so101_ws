// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from so101_msgs:msg/JointCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__TRAITS_HPP_
#define SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "so101_msgs/msg/detail/joint_command__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace so101_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const JointCommand & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: joint_names
  {
    if (msg.joint_names.size() == 0) {
      out << "joint_names: []";
    } else {
      out << "joint_names: [";
      size_t pending_items = msg.joint_names.size();
      for (auto item : msg.joint_names) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: velocities
  {
    if (msg.velocities.size() == 0) {
      out << "velocities: []";
    } else {
      out << "velocities: [";
      size_t pending_items = msg.velocities.size();
      for (auto item : msg.velocities) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: control_mode
  {
    out << "control_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.control_mode, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const JointCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: joint_names
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.joint_names.size() == 0) {
      out << "joint_names: []\n";
    } else {
      out << "joint_names:\n";
      for (auto item : msg.joint_names) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: velocities
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.velocities.size() == 0) {
      out << "velocities: []\n";
    } else {
      out << "velocities:\n";
      for (auto item : msg.velocities) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: control_mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "control_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.control_mode, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const JointCommand & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace so101_msgs

namespace rosidl_generator_traits
{

[[deprecated("use so101_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const so101_msgs::msg::JointCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  so101_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use so101_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const so101_msgs::msg::JointCommand & msg)
{
  return so101_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<so101_msgs::msg::JointCommand>()
{
  return "so101_msgs::msg::JointCommand";
}

template<>
inline const char * name<so101_msgs::msg::JointCommand>()
{
  return "so101_msgs/msg/JointCommand";
}

template<>
struct has_fixed_size<so101_msgs::msg::JointCommand>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<so101_msgs::msg::JointCommand>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<so101_msgs::msg::JointCommand>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__TRAITS_HPP_
