// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from so101_msgs:msg/JointVelocityCommand.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "so101_msgs/msg/detail/joint_velocity_command__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace so101_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void JointVelocityCommand_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) so101_msgs::msg::JointVelocityCommand(_init);
}

void JointVelocityCommand_fini_function(void * message_memory)
{
  auto typed_message = static_cast<so101_msgs::msg::JointVelocityCommand *>(message_memory);
  typed_message->~JointVelocityCommand();
}

size_t size_function__JointVelocityCommand__joint_names(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<std::string> *>(untyped_member);
  return member->size();
}

const void * get_const_function__JointVelocityCommand__joint_names(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<std::string> *>(untyped_member);
  return &member[index];
}

void * get_function__JointVelocityCommand__joint_names(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<std::string> *>(untyped_member);
  return &member[index];
}

void fetch_function__JointVelocityCommand__joint_names(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const std::string *>(
    get_const_function__JointVelocityCommand__joint_names(untyped_member, index));
  auto & value = *reinterpret_cast<std::string *>(untyped_value);
  value = item;
}

void assign_function__JointVelocityCommand__joint_names(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<std::string *>(
    get_function__JointVelocityCommand__joint_names(untyped_member, index));
  const auto & value = *reinterpret_cast<const std::string *>(untyped_value);
  item = value;
}

void resize_function__JointVelocityCommand__joint_names(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<std::string> *>(untyped_member);
  member->resize(size);
}

size_t size_function__JointVelocityCommand__velocities(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<double> *>(untyped_member);
  return member->size();
}

const void * get_const_function__JointVelocityCommand__velocities(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<double> *>(untyped_member);
  return &member[index];
}

void * get_function__JointVelocityCommand__velocities(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<double> *>(untyped_member);
  return &member[index];
}

void fetch_function__JointVelocityCommand__velocities(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__JointVelocityCommand__velocities(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__JointVelocityCommand__velocities(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__JointVelocityCommand__velocities(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

void resize_function__JointVelocityCommand__velocities(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<double> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember JointVelocityCommand_message_member_array[3] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(so101_msgs::msg::JointVelocityCommand, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "joint_names",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(so101_msgs::msg::JointVelocityCommand, joint_names),  // bytes offset in struct
    nullptr,  // default value
    size_function__JointVelocityCommand__joint_names,  // size() function pointer
    get_const_function__JointVelocityCommand__joint_names,  // get_const(index) function pointer
    get_function__JointVelocityCommand__joint_names,  // get(index) function pointer
    fetch_function__JointVelocityCommand__joint_names,  // fetch(index, &value) function pointer
    assign_function__JointVelocityCommand__joint_names,  // assign(index, value) function pointer
    resize_function__JointVelocityCommand__joint_names  // resize(index) function pointer
  },
  {
    "velocities",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(so101_msgs::msg::JointVelocityCommand, velocities),  // bytes offset in struct
    nullptr,  // default value
    size_function__JointVelocityCommand__velocities,  // size() function pointer
    get_const_function__JointVelocityCommand__velocities,  // get_const(index) function pointer
    get_function__JointVelocityCommand__velocities,  // get(index) function pointer
    fetch_function__JointVelocityCommand__velocities,  // fetch(index, &value) function pointer
    assign_function__JointVelocityCommand__velocities,  // assign(index, value) function pointer
    resize_function__JointVelocityCommand__velocities  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers JointVelocityCommand_message_members = {
  "so101_msgs::msg",  // message namespace
  "JointVelocityCommand",  // message name
  3,  // number of fields
  sizeof(so101_msgs::msg::JointVelocityCommand),
  JointVelocityCommand_message_member_array,  // message members
  JointVelocityCommand_init_function,  // function to initialize message memory (memory has to be allocated)
  JointVelocityCommand_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t JointVelocityCommand_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &JointVelocityCommand_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace so101_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<so101_msgs::msg::JointVelocityCommand>()
{
  return &::so101_msgs::msg::rosidl_typesupport_introspection_cpp::JointVelocityCommand_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, so101_msgs, msg, JointVelocityCommand)() {
  return &::so101_msgs::msg::rosidl_typesupport_introspection_cpp::JointVelocityCommand_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
