// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from so101_msgs:msg/CartesianCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__STRUCT_HPP_
#define SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'velocity'
#include "geometry_msgs/msg/detail/twist__struct.hpp"
// Member 'target_pose'
#include "geometry_msgs/msg/detail/pose__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__so101_msgs__msg__CartesianCommand __attribute__((deprecated))
#else
# define DEPRECATED__so101_msgs__msg__CartesianCommand __declspec(deprecated)
#endif

namespace so101_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct CartesianCommand_
{
  using Type = CartesianCommand_<ContainerAllocator>;

  explicit CartesianCommand_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    velocity(_init),
    target_pose(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->mode = "";
      this->valid = false;
    }
  }

  explicit CartesianCommand_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    mode(_alloc),
    velocity(_alloc, _init),
    target_pose(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->mode = "";
      this->valid = false;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _mode_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _mode_type mode;
  using _velocity_type =
    geometry_msgs::msg::Twist_<ContainerAllocator>;
  _velocity_type velocity;
  using _target_pose_type =
    geometry_msgs::msg::Pose_<ContainerAllocator>;
  _target_pose_type target_pose;
  using _valid_type =
    bool;
  _valid_type valid;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__mode(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->mode = _arg;
    return *this;
  }
  Type & set__velocity(
    const geometry_msgs::msg::Twist_<ContainerAllocator> & _arg)
  {
    this->velocity = _arg;
    return *this;
  }
  Type & set__target_pose(
    const geometry_msgs::msg::Pose_<ContainerAllocator> & _arg)
  {
    this->target_pose = _arg;
    return *this;
  }
  Type & set__valid(
    const bool & _arg)
  {
    this->valid = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    so101_msgs::msg::CartesianCommand_<ContainerAllocator> *;
  using ConstRawPtr =
    const so101_msgs::msg::CartesianCommand_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<so101_msgs::msg::CartesianCommand_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<so101_msgs::msg::CartesianCommand_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      so101_msgs::msg::CartesianCommand_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<so101_msgs::msg::CartesianCommand_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      so101_msgs::msg::CartesianCommand_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<so101_msgs::msg::CartesianCommand_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<so101_msgs::msg::CartesianCommand_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<so101_msgs::msg::CartesianCommand_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__so101_msgs__msg__CartesianCommand
    std::shared_ptr<so101_msgs::msg::CartesianCommand_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__so101_msgs__msg__CartesianCommand
    std::shared_ptr<so101_msgs::msg::CartesianCommand_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const CartesianCommand_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->mode != other.mode) {
      return false;
    }
    if (this->velocity != other.velocity) {
      return false;
    }
    if (this->target_pose != other.target_pose) {
      return false;
    }
    if (this->valid != other.valid) {
      return false;
    }
    return true;
  }
  bool operator!=(const CartesianCommand_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct CartesianCommand_

// alias to use template instance with default allocator
using CartesianCommand =
  so101_msgs::msg::CartesianCommand_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace so101_msgs

#endif  // SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__STRUCT_HPP_
