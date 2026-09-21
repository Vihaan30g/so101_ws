// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from so101_msgs:msg/JointVelocityCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__JOINT_VELOCITY_COMMAND__STRUCT_H_
#define SO101_MSGS__MSG__DETAIL__JOINT_VELOCITY_COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'joint_names'
#include "rosidl_runtime_c/string.h"
// Member 'velocities'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/JointVelocityCommand in the package so101_msgs.
/**
  * For the directly-teleoperated wrist joints j4/j5.
 */
typedef struct so101_msgs__msg__JointVelocityCommand
{
  std_msgs__msg__Header header;
  /// ["j4","j5"]
  rosidl_runtime_c__String__Sequence joint_names;
  rosidl_runtime_c__double__Sequence velocities;
} so101_msgs__msg__JointVelocityCommand;

// Struct for a sequence of so101_msgs__msg__JointVelocityCommand.
typedef struct so101_msgs__msg__JointVelocityCommand__Sequence
{
  so101_msgs__msg__JointVelocityCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} so101_msgs__msg__JointVelocityCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SO101_MSGS__MSG__DETAIL__JOINT_VELOCITY_COMMAND__STRUCT_H_
