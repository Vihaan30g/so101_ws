// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from so101_msgs:msg/CartesianCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__STRUCT_H_
#define SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__STRUCT_H_

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
// Member 'mode'
#include "rosidl_runtime_c/string.h"
// Member 'velocity'
#include "geometry_msgs/msg/detail/twist__struct.h"
// Member 'target_pose'
#include "geometry_msgs/msg/detail/pose__struct.h"

/// Struct defined in msg/CartesianCommand in the package so101_msgs.
/**
  * Output of ANY input node (keyboard today, ArUco/IMU later), input of the controller.
 */
typedef struct so101_msgs__msg__CartesianCommand
{
  std_msgs__msg__Header header;
  /// "velocity" or "pose"   (start with "velocity" for keyboard)
  rosidl_runtime_c__String mode;
  /// linear part used; angular part ignored (translation-only control)
  geometry_msgs__msg__Twist velocity;
  /// only used if mode == "pose"
  geometry_msgs__msg__Pose target_pose;
  /// false if the input node itself detected bad/stale data
  bool valid;
} so101_msgs__msg__CartesianCommand;

// Struct for a sequence of so101_msgs__msg__CartesianCommand.
typedef struct so101_msgs__msg__CartesianCommand__Sequence
{
  so101_msgs__msg__CartesianCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} so101_msgs__msg__CartesianCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__STRUCT_H_
