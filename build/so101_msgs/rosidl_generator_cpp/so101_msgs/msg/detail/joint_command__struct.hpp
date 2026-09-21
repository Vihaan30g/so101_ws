// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from so101_msgs:msg/JointCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__STRUCT_HPP_
#define SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__STRUCT_HPP_

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
# define DEPRECATED__so101_msgs__msg__JointCommand __attribute__((deprecated))
#else
# define DEPRECATED__so101_msgs__msg__JointCommand __declspec(deprecated)
#endif

namespace so101_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct JointCommand_
{
  using Type = JointCommand_<ContainerAllocator>;

  explicit JointCommand_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->control_mode = 0;
    }
  }

  explicit JointCommand_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->control_mode = 0;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _joint_names_type =
    std::vector<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>>>;
  _joint_names_type joint_names;
  using _velocities_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _velocities_type velocities;
  using _control_mode_type =
    uint8_t;
  _control_mode_type control_mode;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__joint_names(
    const std::vector<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>>> & _arg)
  {
    this->joint_names = _arg;
    return *this;
  }
  Type & set__velocities(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->velocities = _arg;
    return *this;
  }
  Type & set__control_mode(
    const uint8_t & _arg)
  {
    this->control_mode = _arg;
    return *this;
  }

  // constant declarations
  static constexpr uint8_t NORMAL =
    0u;
  static constexpr uint8_t FROZEN_TRANSLATION =
    1u;

  // pointer types
  using RawPtr =
    so101_msgs::msg::JointCommand_<ContainerAllocator> *;
  using ConstRawPtr =
    const so101_msgs::msg::JointCommand_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<so101_msgs::msg::JointCommand_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<so101_msgs::msg::JointCommand_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      so101_msgs::msg::JointCommand_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<so101_msgs::msg::JointCommand_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      so101_msgs::msg::JointCommand_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<so101_msgs::msg::JointCommand_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<so101_msgs::msg::JointCommand_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<so101_msgs::msg::JointCommand_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__so101_msgs__msg__JointCommand
    std::shared_ptr<so101_msgs::msg::JointCommand_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__so101_msgs__msg__JointCommand
    std::shared_ptr<so101_msgs::msg::JointCommand_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const JointCommand_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->joint_names != other.joint_names) {
      return false;
    }
    if (this->velocities != other.velocities) {
      return false;
    }
    if (this->control_mode != other.control_mode) {
      return false;
    }
    return true;
  }
  bool operator!=(const JointCommand_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct JointCommand_

// alias to use template instance with default allocator
using JointCommand =
  so101_msgs::msg::JointCommand_<std::allocator<void>>;

// constant definitions
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t JointCommand_<ContainerAllocator>::NORMAL;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t JointCommand_<ContainerAllocator>::FROZEN_TRANSLATION;
#endif  // __cplusplus < 201703L

}  // namespace msg

}  // namespace so101_msgs

#endif  // SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__STRUCT_HPP_
