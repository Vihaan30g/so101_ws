// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from so101_msgs:msg/JointCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__BUILDER_HPP_
#define SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "so101_msgs/msg/detail/joint_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace so101_msgs
{

namespace msg
{

namespace builder
{

class Init_JointCommand_control_mode
{
public:
  explicit Init_JointCommand_control_mode(::so101_msgs::msg::JointCommand & msg)
  : msg_(msg)
  {}
  ::so101_msgs::msg::JointCommand control_mode(::so101_msgs::msg::JointCommand::_control_mode_type arg)
  {
    msg_.control_mode = std::move(arg);
    return std::move(msg_);
  }

private:
  ::so101_msgs::msg::JointCommand msg_;
};

class Init_JointCommand_velocities
{
public:
  explicit Init_JointCommand_velocities(::so101_msgs::msg::JointCommand & msg)
  : msg_(msg)
  {}
  Init_JointCommand_control_mode velocities(::so101_msgs::msg::JointCommand::_velocities_type arg)
  {
    msg_.velocities = std::move(arg);
    return Init_JointCommand_control_mode(msg_);
  }

private:
  ::so101_msgs::msg::JointCommand msg_;
};

class Init_JointCommand_joint_names
{
public:
  explicit Init_JointCommand_joint_names(::so101_msgs::msg::JointCommand & msg)
  : msg_(msg)
  {}
  Init_JointCommand_velocities joint_names(::so101_msgs::msg::JointCommand::_joint_names_type arg)
  {
    msg_.joint_names = std::move(arg);
    return Init_JointCommand_velocities(msg_);
  }

private:
  ::so101_msgs::msg::JointCommand msg_;
};

class Init_JointCommand_header
{
public:
  Init_JointCommand_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_JointCommand_joint_names header(::so101_msgs::msg::JointCommand::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_JointCommand_joint_names(msg_);
  }

private:
  ::so101_msgs::msg::JointCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::so101_msgs::msg::JointCommand>()
{
  return so101_msgs::msg::builder::Init_JointCommand_header();
}

}  // namespace so101_msgs

#endif  // SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__BUILDER_HPP_
