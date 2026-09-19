// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from so101_msgs:msg/CartesianCommand.idl
// generated code does not contain a copyright notice

#ifndef SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__FUNCTIONS_H_
#define SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "so101_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "so101_msgs/msg/detail/cartesian_command__struct.h"

/// Initialize msg/CartesianCommand message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * so101_msgs__msg__CartesianCommand
 * )) before or use
 * so101_msgs__msg__CartesianCommand__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
bool
so101_msgs__msg__CartesianCommand__init(so101_msgs__msg__CartesianCommand * msg);

/// Finalize msg/CartesianCommand message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
void
so101_msgs__msg__CartesianCommand__fini(so101_msgs__msg__CartesianCommand * msg);

/// Create msg/CartesianCommand message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * so101_msgs__msg__CartesianCommand__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
so101_msgs__msg__CartesianCommand *
so101_msgs__msg__CartesianCommand__create();

/// Destroy msg/CartesianCommand message.
/**
 * It calls
 * so101_msgs__msg__CartesianCommand__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
void
so101_msgs__msg__CartesianCommand__destroy(so101_msgs__msg__CartesianCommand * msg);

/// Check for msg/CartesianCommand message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
bool
so101_msgs__msg__CartesianCommand__are_equal(const so101_msgs__msg__CartesianCommand * lhs, const so101_msgs__msg__CartesianCommand * rhs);

/// Copy a msg/CartesianCommand message.
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
so101_msgs__msg__CartesianCommand__copy(
  const so101_msgs__msg__CartesianCommand * input,
  so101_msgs__msg__CartesianCommand * output);

/// Initialize array of msg/CartesianCommand messages.
/**
 * It allocates the memory for the number of elements and calls
 * so101_msgs__msg__CartesianCommand__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
bool
so101_msgs__msg__CartesianCommand__Sequence__init(so101_msgs__msg__CartesianCommand__Sequence * array, size_t size);

/// Finalize array of msg/CartesianCommand messages.
/**
 * It calls
 * so101_msgs__msg__CartesianCommand__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
void
so101_msgs__msg__CartesianCommand__Sequence__fini(so101_msgs__msg__CartesianCommand__Sequence * array);

/// Create array of msg/CartesianCommand messages.
/**
 * It allocates the memory for the array and calls
 * so101_msgs__msg__CartesianCommand__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
so101_msgs__msg__CartesianCommand__Sequence *
so101_msgs__msg__CartesianCommand__Sequence__create(size_t size);

/// Destroy array of msg/CartesianCommand messages.
/**
 * It calls
 * so101_msgs__msg__CartesianCommand__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
void
so101_msgs__msg__CartesianCommand__Sequence__destroy(so101_msgs__msg__CartesianCommand__Sequence * array);

/// Check for msg/CartesianCommand message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_so101_msgs
bool
so101_msgs__msg__CartesianCommand__Sequence__are_equal(const so101_msgs__msg__CartesianCommand__Sequence * lhs, const so101_msgs__msg__CartesianCommand__Sequence * rhs);

/// Copy an array of msg/CartesianCommand messages.
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
so101_msgs__msg__CartesianCommand__Sequence__copy(
  const so101_msgs__msg__CartesianCommand__Sequence * input,
  so101_msgs__msg__CartesianCommand__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // SO101_MSGS__MSG__DETAIL__CARTESIAN_COMMAND__FUNCTIONS_H_
