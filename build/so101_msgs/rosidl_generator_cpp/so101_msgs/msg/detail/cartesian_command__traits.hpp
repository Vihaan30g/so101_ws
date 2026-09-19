// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from so101_msgs:msg/CartesianCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__TRAITS_HPP_
#define SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "so101_msgs/msg/detail/cartesian_command__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'velocity'
#include "geometry_msgs/msg/detail/twist__traits.hpp"
// Member 'target_pose'
#include "geometry_msgs/msg/detail/pose__traits.hpp"

namespace so101_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const CartesianCommand & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: mode
  {
    out << "mode: ";
    rosidl_generator_traits::value_to_yaml(msg.mode, out);
    out << ", ";
  }

  // member: velocity
  {
    out << "velocity: ";
    to_flow_style_yaml(msg.velocity, out);
    out << ", ";
  }

  // member: target_pose
  {
    out << "target_pose: ";
    to_flow_style_yaml(msg.target_pose, out);
    out << ", ";
  }

  // member: valid
  {
    out << "valid: ";
    rosidl_generator_traits::value_to_yaml(msg.valid, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const CartesianCommand & msg,
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

  // member: mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "mode: ";
    rosidl_generator_traits::value_to_yaml(msg.mode, out);
    out << "\n";
  }

  // member: velocity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "velocity:\n";
    to_block_style_yaml(msg.velocity, out, indentation + 2);
  }

  // member: target_pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "target_pose:\n";
    to_block_style_yaml(msg.target_pose, out, indentation + 2);
  }

  // member: valid
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "valid: ";
    rosidl_generator_traits::value_to_yaml(msg.valid, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const CartesianCommand & msg, bool use_flow_style = false)
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
  const so101_msgs::msg::CartesianCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  so101_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use so101_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const so101_msgs::msg::CartesianCommand & msg)
{
  return so101_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<so101_msgs::msg::CartesianCommand>()
{
  return "so101_msgs::msg::CartesianCommand";
}

template<>
inline const char * name<so101_msgs::msg::CartesianCommand>()
{
  return "so101_msgs/msg/CartesianCommand";
}

template<>
struct has_fixed_size<so101_msgs::msg::CartesianCommand>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<so101_msgs::msg::CartesianCommand>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<so101_msgs::msg::CartesianCommand>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__TRAITS_HPP_
