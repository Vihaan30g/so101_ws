// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from so101_msgs:msg/ControllerStatus.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__TRAITS_HPP_
#define SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "so101_msgs/msg/detail/controller_status__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace so101_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const ControllerStatus & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: in_singularity
  {
    out << "in_singularity: ";
    rosidl_generator_traits::value_to_yaml(msg.in_singularity, out);
    out << ", ";
  }

  // member: manipulability_index
  {
    out << "manipulability_index: ";
    rosidl_generator_traits::value_to_yaml(msg.manipulability_index, out);
    out << ", ";
  }

  // member: near_joint_limit
  {
    out << "near_joint_limit: ";
    rosidl_generator_traits::value_to_yaml(msg.near_joint_limit, out);
    out << ", ";
  }

  // member: self_collision_detected
  {
    out << "self_collision_detected: ";
    rosidl_generator_traits::value_to_yaml(msg.self_collision_detected, out);
    out << ", ";
  }

  // member: last_error
  {
    out << "last_error: ";
    rosidl_generator_traits::value_to_yaml(msg.last_error, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ControllerStatus & msg,
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

  // member: in_singularity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "in_singularity: ";
    rosidl_generator_traits::value_to_yaml(msg.in_singularity, out);
    out << "\n";
  }

  // member: manipulability_index
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "manipulability_index: ";
    rosidl_generator_traits::value_to_yaml(msg.manipulability_index, out);
    out << "\n";
  }

  // member: near_joint_limit
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "near_joint_limit: ";
    rosidl_generator_traits::value_to_yaml(msg.near_joint_limit, out);
    out << "\n";
  }

  // member: self_collision_detected
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "self_collision_detected: ";
    rosidl_generator_traits::value_to_yaml(msg.self_collision_detected, out);
    out << "\n";
  }

  // member: last_error
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "last_error: ";
    rosidl_generator_traits::value_to_yaml(msg.last_error, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ControllerStatus & msg, bool use_flow_style = false)
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
  const so101_msgs::msg::ControllerStatus & msg,
  std::ostream & out, size_t indentation = 0)
{
  so101_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use so101_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const so101_msgs::msg::ControllerStatus & msg)
{
  return so101_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<so101_msgs::msg::ControllerStatus>()
{
  return "so101_msgs::msg::ControllerStatus";
}

template<>
inline const char * name<so101_msgs::msg::ControllerStatus>()
{
  return "so101_msgs/msg/ControllerStatus";
}

template<>
struct has_fixed_size<so101_msgs::msg::ControllerStatus>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<so101_msgs::msg::ControllerStatus>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<so101_msgs::msg::ControllerStatus>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__TRAITS_HPP_
