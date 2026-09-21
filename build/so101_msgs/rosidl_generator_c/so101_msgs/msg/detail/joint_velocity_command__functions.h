// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from so101_msgs:msg/JointVelocityCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__JOINT_VELOCITY_COMMAND__FUNCTIONS_H_
#define SO101_MSGS__MSG__DETAIL__JOINT_VELOCITY_COMMAND__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "so101_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "so101_msgs/msg/detail/joint_velocity_command__struct.h"

/// Initialize msg/JointVelocityCommand message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * so101_msgs__msg__JointVelocityCommand
 * )) before or use
 * so101_msgs__msg__JointVelocityCommand__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
bool
so101_msgs__msg__JointVelocityCommand__init(so101_msgs__msg__JointVelocityCommand * msg);

/// Finalize msg/JointVelocityCommand message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
void
so101_msgs__msg__JointVelocityCommand__fini(so101_msgs__msg__JointVelocityCommand * msg);

/// Create msg/JointVelocityCommand message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * so101_msgs__msg__JointVelocityCommand__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
so101_msgs__msg__JointVelocityCommand *
so101_msgs__msg__JointVelocityCommand__create();

/// Destroy msg/JointVelocityCommand message.
/**
 * It calls
 * so101_msgs__msg__JointVelocityCommand__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
void
so101_msgs__msg__JointVelocityCommand__destroy(so101_msgs__msg__JointVelocityCommand * msg);

/// Check for msg/JointVelocityCommand message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
bool
so101_msgs__msg__JointVelocityCommand__are_equal(const so101_msgs__msg__JointVelocityCommand * lhs, const so101_msgs__msg__JointVelocityCommand * rhs);

/// Copy a msg/JointVelocityCommand message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
bool
so101_msgs__msg__JointVelocityCommand__copy(
  const so101_msgs__msg__JointVelocityCommand * input,
  so101_msgs__msg__JointVelocityCommand * output);

/// Initialize array of msg/JointVelocityCommand messages.
/**
 * It allocates the memory for the number of elements and calls
 * so101_msgs__msg__JointVelocityCommand__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
bool
so101_msgs__msg__JointVelocityCommand__Sequence__init(so101_msgs__msg__JointVelocityCommand__Sequence * array, size_t size);

/// Finalize array of msg/JointVelocityCommand messages.
/**
 * It calls
 * so101_msgs__msg__JointVelocityCommand__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
void
so101_msgs__msg__JointVelocityCommand__Sequence__fini(so101_msgs__msg__JointVelocityCommand__Sequence * array);

/// Create array of msg/JointVelocityCommand messages.
/**
 * It allocates the memory for the array and calls
 * so101_msgs__msg__JointVelocityCommand__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
so101_msgs__msg__JointVelocityCommand__Sequence *
so101_msgs__msg__JointVelocityCommand__Sequence__create(size_t size);

/// Destroy array of msg/JointVelocityCommand messages.
/**
 * It calls
 * so101_msgs__msg__JointVelocityCommand__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
void
so101_msgs__msg__JointVelocityCommand__Sequence__destroy(so101_msgs__msg__JointVelocityCommand__Sequence * array);

/// Check for msg/JointVelocityCommand message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
bool
so101_msgs__msg__JointVelocityCommand__Sequence__are_equal(const so101_msgs__msg__JointVelocityCommand__Sequence * lhs, const so101_msgs__msg__JointVelocityCommand__Sequence * rhs);

/// Copy an array of msg/JointVelocityCommand messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
bool
so101_msgs__msg__JointVelocityCommand__Sequence__copy(
  const so101_msgs__msg__JointVelocityCommand__Sequence * input,
  so101_msgs__msg__JointVelocityCommand__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // SO101_MSGS__MSG__DETAIL__JOINT_VELOCITY_COMMAND__FUNCTIONS_H_
