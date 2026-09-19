// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from so101_msgs:msg/CartesianCommand.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "so101_msgs/msg/detail/cartesian_command__rosidl_typesupport_introspection_c.h"
#include "so101_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "so101_msgs/msg/detail/cartesian_command__functions.h"
#include "so101_msgs/msg/detail/cartesian_command__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `mode`
#include "rosidl_runtime_c/string_functions.h"
// Member `velocity`
#include "geometry_msgs/msg/twist.h"
// Member `velocity`
#include "geometry_msgs/msg/detail/twist__rosidl_typesupport_introspection_c.h"
// Member `target_pose`
#include "geometry_msgs/msg/pose.h"
// Member `target_pose`
#include "geometry_msgs/msg/detail/pose__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  so101_msgs__msg__CartesianCommand__init(message_memory);
}

void so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_fini_function(void * message_memory)
{
  so101_msgs__msg__CartesianCommand__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_member_array[5] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(so101_msgs__msg__CartesianCommand, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "mode",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(so101_msgs__msg__CartesianCommand, mode),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "velocity",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(so101_msgs__msg__CartesianCommand, velocity),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "target_pose",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(so101_msgs__msg__CartesianCommand, target_pose),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "valid",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(so101_msgs__msg__CartesianCommand, valid),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_members = {
  "so101_msgs__msg",  // message namespace
  "CartesianCommand",  // message name
  5,  // number of fields
  sizeof(so101_msgs__msg__CartesianCommand),
  so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_member_array,  // message members
  so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_init_function,  // function to initialize message memory (memory has to be allocated)
  so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_type_support_handle = {
  0,
  &so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_so101_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, so101_msgs, msg, CartesianCommand)() {
  so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Twist)();
  so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_member_array[3].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Pose)();
  if (!so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_type_support_handle.typesupport_identifier) {
    so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &so101_msgs__msg__CartesianCommand__rosidl_typesupport_introspection_c__CartesianCommand_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
