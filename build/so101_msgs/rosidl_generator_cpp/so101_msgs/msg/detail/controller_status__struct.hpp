// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from so101_msgs:msg/ControllerStatus.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__STRUCT_HPP_
#define SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__STRUCT_HPP_

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

#ifndef _WIN32
# define DEPRECATED__so101_msgs__msg__ControllerStatus __attribute__((deprecated))
#else
# define DEPRECATED__so101_msgs__msg__ControllerStatus __declspec(deprecated)
#endif

namespace so101_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ControllerStatus_
{
  using Type = ControllerStatus_<ContainerAllocator>;

  explicit ControllerStatus_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->in_singularity = false;
      this->manipulability_index = 0.0;
      this->near_joint_limit = false;
      this->self_collision_detected = false;
      this->last_error = "";
    }
  }

  explicit ControllerStatus_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    last_error(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->in_singularity = false;
      this->manipulability_index = 0.0;
      this->near_joint_limit = false;
      this->self_collision_detected = false;
      this->last_error = "";
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _in_singularity_type =
    bool;
  _in_singularity_type in_singularity;
  using _manipulability_index_type =
    double;
  _manipulability_index_type manipulability_index;
  using _near_joint_limit_type =
    bool;
  _near_joint_limit_type near_joint_limit;
  using _self_collision_detected_type =
    bool;
  _self_collision_detected_type self_collision_detected;
  using _last_error_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _last_error_type last_error;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__in_singularity(
    const bool & _arg)
  {
    this->in_singularity = _arg;
    return *this;
  }
  Type & set__manipulability_index(
    const double & _arg)
  {
    this->manipulability_index = _arg;
    return *this;
  }
  Type & set__near_joint_limit(
    const bool & _arg)
  {
    this->near_joint_limit = _arg;
    return *this;
  }
  Type & set__self_collision_detected(
    const bool & _arg)
  {
    this->self_collision_detected = _arg;
    return *this;
  }
  Type & set__last_error(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->last_error = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    so101_msgs::msg::ControllerStatus_<ContainerAllocator> *;
  using ConstRawPtr =
    const so101_msgs::msg::ControllerStatus_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<so101_msgs::msg::ControllerStatus_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<so101_msgs::msg::ControllerStatus_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      so101_msgs::msg::ControllerStatus_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<so101_msgs::msg::ControllerStatus_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      so101_msgs::msg::ControllerStatus_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<so101_msgs::msg::ControllerStatus_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<so101_msgs::msg::ControllerStatus_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<so101_msgs::msg::ControllerStatus_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__so101_msgs__msg__ControllerStatus
    std::shared_ptr<so101_msgs::msg::ControllerStatus_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__so101_msgs__msg__ControllerStatus
    std::shared_ptr<so101_msgs::msg::ControllerStatus_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ControllerStatus_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->in_singularity != other.in_singularity) {
      return false;
    }
    if (this->manipulability_index != other.manipulability_index) {
      return false;
    }
    if (this->near_joint_limit != other.near_joint_limit) {
      return false;
    }
    if (this->self_collision_detected != other.self_collision_detected) {
      return false;
    }
    if (this->last_error != other.last_error) {
      return false;
    }
    return true;
  }
  bool operator!=(const ControllerStatus_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ControllerStatus_

// alias to use template instance with default allocator
using ControllerStatus =
  so101_msgs::msg::ControllerStatus_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace so101_msgs

#endif  // SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__STRUCT_HPP_
