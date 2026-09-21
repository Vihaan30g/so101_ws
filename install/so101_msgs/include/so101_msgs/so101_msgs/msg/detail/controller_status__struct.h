// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from so101_msgs:msg/ControllerStatus.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__STRUCT_H_
#define SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__STRUCT_H_

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
// Member 'last_error'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/ControllerStatus in the package so101_msgs.
/**
  * Richer, structured companion to the plain diagnostic_msgs/DiagnosticArray stream.
 */
typedef struct so101_msgs__msg__ControllerStatus
{
  std_msgs__msg__Header header;
  bool in_singularity;
  double manipulability_index;
  bool near_joint_limit;
  bool self_collision_detected;
  /// empty string if healthy
  rosidl_runtime_c__String last_error;
} so101_msgs__msg__ControllerStatus;

// Struct for a sequence of so101_msgs__msg__ControllerStatus.
typedef struct so101_msgs__msg__ControllerStatus__Sequence
{
  so101_msgs__msg__ControllerStatus * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} so101_msgs__msg__ControllerStatus__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SO101_MSGS__MSG__DETAIL__CONTROLLER_STATUS__STRUCT_H_
