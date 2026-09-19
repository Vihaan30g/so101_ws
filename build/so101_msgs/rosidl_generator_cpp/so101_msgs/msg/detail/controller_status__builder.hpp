// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from so101_msgs:msg/ControllerStatus.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__BUILDER_HPP_
#define SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "so101_msgs/msg/detail/controller_status__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace so101_msgs
{

namespace msg
{

namespace builder
{

class Init_ControllerStatus_last_error
{
public:
  explicit Init_ControllerStatus_last_error(::so101_msgs::msg::ControllerStatus & msg)
  : msg_(msg)
  {}
  ::so101_msgs::msg::ControllerStatus last_error(::so101_msgs::msg::ControllerStatus::_last_error_type arg)
  {
    msg_.last_error = std::move(arg);
    return std::move(msg_);
  }

private:
  ::so101_msgs::msg::ControllerStatus msg_;
};

class Init_ControllerStatus_self_collision_detected
{
public:
  explicit Init_ControllerStatus_self_collision_detected(::so101_msgs::msg::ControllerStatus & msg)
  : msg_(msg)
  {}
  Init_ControllerStatus_last_error self_collision_detected(::so101_msgs::msg::ControllerStatus::_self_collision_detected_type arg)
  {
    msg_.self_collision_detected = std::move(arg);
    return Init_ControllerStatus_last_error(msg_);
  }

private:
  ::so101_msgs::msg::ControllerStatus msg_;
};

class Init_ControllerStatus_near_joint_limit
{
public:
  explicit Init_ControllerStatus_near_joint_limit(::so101_msgs::msg::ControllerStatus & msg)
  : msg_(msg)
  {}
  Init_ControllerStatus_self_collision_detected near_joint_limit(::so101_msgs::msg::ControllerStatus::_near_joint_limit_type arg)
  {
    msg_.near_joint_limit = std::move(arg);
    return Init_ControllerStatus_self_collision_detected(msg_);
  }

private:
  ::so101_msgs::msg::ControllerStatus msg_;
};

class Init_ControllerStatus_manipulability_index
{
public:
  explicit Init_ControllerStatus_manipulability_index(::so101_msgs::msg::ControllerStatus & msg)
  : msg_(msg)
  {}
  Init_ControllerStatus_near_joint_limit manipulability_index(::so101_msgs::msg::ControllerStatus::_manipulability_index_type arg)
  {
    msg_.manipulability_index = std::move(arg);
    return Init_ControllerStatus_near_joint_limit(msg_);
  }

private:
  ::so101_msgs::msg::ControllerStatus msg_;
};

class Init_ControllerStatus_in_singularity
{
public:
  explicit Init_ControllerStatus_in_singularity(::so101_msgs::msg::ControllerStatus & msg)
  : msg_(msg)
  {}
  Init_ControllerStatus_manipulability_index in_singularity(::so101_msgs::msg::ControllerStatus::_in_singularity_type arg)
  {
    msg_.in_singularity = std::move(arg);
    return Init_ControllerStatus_manipulability_index(msg_);
  }

private:
  ::so101_msgs::msg::ControllerStatus msg_;
};

class Init_ControllerStatus_header
{
public:
  Init_ControllerStatus_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ControllerStatus_in_singularity header(::so101_msgs::msg::ControllerStatus::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ControllerStatus_in_singularity(msg_);
  }

private:
  ::so101_msgs::msg::ControllerStatus msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::so101_msgs::msg::ControllerStatus>()
{
  return so101_msgs::msg::builder::Init_ControllerStatus_header();
}

}  // namespace so101_msgs

#endif  // SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__BUILDER_HPP_
