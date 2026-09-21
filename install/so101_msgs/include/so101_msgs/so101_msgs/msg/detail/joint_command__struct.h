// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from so101_msgs:msg/JointCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__STRUCT_H_
#define SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Constant 'NORMAL'.
enum
{
  so101_msgs__msg__JointCommand__NORMAL = 0
};

/// Constant 'FROZEN_TRANSLATION'.
enum
{
  so101_msgs__msg__JointCommand__FROZEN_TRANSLATION = 1
};

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'joint_names'
#include "rosidl_runtime_c/string.h"
// Member 'velocities'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/JointCommand in the package so101_msgs.
/**
  * Full 6-joint output of controller / input of safety gate / input of hardware interface.
  * Real joint names (not the earlier j1..j5 placeholder), kinematic-chain order:
  *   ["shoulder_pan","shoulder_lift","elbow_flex","wrist_flex","wrist_roll","gripper"]
  * gripper is direct-teleop passthrough, same as wrist_flex/wrist_roll -- not IK-controlled.
 */
typedef struct so101_msgs__msg__JointCommand
{
  std_msgs__msg__Header header;
  /// ["shoulder_pan","shoulder_lift","elbow_flex","wrist_flex","wrist_roll","gripper"]
  rosidl_runtime_c__String__Sequence joint_names;
  rosidl_runtime_c__double__Sequence velocities;
  /// NORMAL or FROZEN_TRANSLATION
  uint8_t control_mode;
} so101_msgs__msg__JointCommand;

// Struct for a sequence of so101_msgs__msg__JointCommand.
typedef struct so101_msgs__msg__JointCommand__Sequence
{
  so101_msgs__msg__JointCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} so101_msgs__msg__JointCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SO101_MSGS__MSG__DETAIL__JOINT_COMMAND__STRUCT_H_
