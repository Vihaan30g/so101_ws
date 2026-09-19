// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from so101_msgs:msg/CartesianCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__BUILDER_HPP_
#define SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "so101_msgs/msg/detail/cartesian_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace so101_msgs
{

namespace msg
{

namespace builder
{

class Init_CartesianCommand_valid
{
public:
  explicit Init_CartesianCommand_valid(::so101_msgs::msg::CartesianCommand & msg)
  : msg_(msg)
  {}
  ::so101_msgs::msg::CartesianCommand valid(::so101_msgs::msg::CartesianCommand::_valid_type arg)
  {
    msg_.valid = std::move(arg);
    return std::move(msg_);
  }

private:
  ::so101_msgs::msg::CartesianCommand msg_;
};

class Init_CartesianCommand_target_pose
{
public:
  explicit Init_CartesianCommand_target_pose(::so101_msgs::msg::CartesianCommand & msg)
  : msg_(msg)
  {}
  Init_CartesianCommand_valid target_pose(::so101_msgs::msg::CartesianCommand::_target_pose_type arg)
  {
    msg_.target_pose = std::move(arg);
    return Init_CartesianCommand_valid(msg_);
  }

private:
  ::so101_msgs::msg::CartesianCommand msg_;
};

class Init_CartesianCommand_velocity
{
public:
  explicit Init_CartesianCommand_velocity(::so101_msgs::msg::CartesianCommand & msg)
  : msg_(msg)
  {}
  Init_CartesianCommand_target_pose velocity(::so101_msgs::msg::CartesianCommand::_velocity_type arg)
  {
    msg_.velocity = std::move(arg);
    return Init_CartesianCommand_target_pose(msg_);
  }

private:
  ::so101_msgs::msg::CartesianCommand msg_;
};

class Init_CartesianCommand_mode
{
public:
  explicit Init_CartesianCommand_mode(::so101_msgs::msg::CartesianCommand & msg)
  : msg_(msg)
  {}
  Init_CartesianCommand_velocity mode(::so101_msgs::msg::CartesianCommand::_mode_type arg)
  {
    msg_.mode = std::move(arg);
    return Init_CartesianCommand_velocity(msg_);
  }

private:
  ::so101_msgs::msg::CartesianCommand msg_;
};

class Init_CartesianCommand_header
{
public:
  Init_CartesianCommand_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CartesianCommand_mode header(::so101_msgs::msg::CartesianCommand::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_CartesianCommand_mode(msg_);
  }

private:
  ::so101_msgs::msg::CartesianCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::so101_msgs::msg::CartesianCommand>()
{
  return so101_msgs::msg::builder::Init_CartesianCommand_header();
}

}  // namespace so101_msgs

#endif  // SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__BUILDER_HPP_
