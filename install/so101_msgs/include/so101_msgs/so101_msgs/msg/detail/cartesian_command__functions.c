// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from so101_msgs:msg/CartesianCommand.idl
// generated code does not contain a copyright notice
#include "so101_msgs/msg/detail/cartesian_command__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `mode`
#include "rosidl_runtime_c/string_functions.h"
// Member `velocity`
#include "geometry_msgs/msg/detail/twist__functions.h"
// Member `target_pose`
#include "geometry_msgs/msg/detail/pose__functions.h"

bool
so101_msgs__msg__CartesianCommand__init(so101_msgs__msg__CartesianCommand * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    so101_msgs__msg__CartesianCommand__fini(msg);
    return false;
  }
  // mode
  if (!rosidl_runtime_c__String__init(&msg->mode)) {
    so101_msgs__msg__CartesianCommand__fini(msg);
    return false;
  }
  // velocity
  if (!geometry_msgs__msg__Twist__init(&msg->velocity)) {
    so101_msgs__msg__CartesianCommand__fini(msg);
    return false;
  }
  // target_pose
  if (!geometry_msgs__msg__Pose__init(&msg->target_pose)) {
    so101_msgs__msg__CartesianCommand__fini(msg);
    return false;
  }
  // valid
  return true;
}

void
so101_msgs__msg__CartesianCommand__fini(so101_msgs__msg__CartesianCommand * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // mode
  rosidl_runtime_c__String__fini(&msg->mode);
  // velocity
  geometry_msgs__msg__Twist__fini(&msg->velocity);
  // target_pose
  geometry_msgs__msg__Pose__fini(&msg->target_pose);
  // valid
}

bool
so101_msgs__msg__CartesianCommand__are_equal(const so101_msgs__msg__CartesianCommand * lhs, const so101_msgs__msg__CartesianCommand * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // mode
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->mode), &(rhs->mode)))
  {
    return false;
  }
  // velocity
  if (!geometry_msgs__msg__Twist__are_equal(
      &(lhs->velocity), &(rhs->velocity)))
  {
    return false;
  }
  // target_pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->target_pose), &(rhs->target_pose)))
  {
    return false;
  }
  // valid
  if (lhs->valid != rhs->valid) {
    return false;
  }
  return true;
}

bool
so101_msgs__msg__CartesianCommand__copy(
  const so101_msgs__msg__CartesianCommand * input,
  so101_msgs__msg__CartesianCommand * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // mode
  if (!rosidl_runtime_c__String__copy(
      &(input->mode), &(output->mode)))
  {
    return false;
  }
  // velocity
  if (!geometry_msgs__msg__Twist__copy(
      &(input->velocity), &(output->velocity)))
  {
    return false;
  }
  // target_pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->target_pose), &(output->target_pose)))
  {
    return false;
  }
  // valid
  output->valid = input->valid;
  return true;
}

so101_msgs__msg__CartesianCommand *
so101_msgs__msg__CartesianCommand__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  so101_msgs__msg__CartesianCommand * msg = (so101_msgs__msg__CartesianCommand *)allocator.allocate(sizeof(so101_msgs__msg__CartesianCommand), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(so101_msgs__msg__CartesianCommand));
  bool success = so101_msgs__msg__CartesianCommand__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
so101_msgs__msg__CartesianCommand__destroy(so101_msgs__msg__CartesianCommand * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    so101_msgs__msg__CartesianCommand__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
so101_msgs__msg__CartesianCommand__Sequence__init(so101_msgs__msg__CartesianCommand__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  so101_msgs__msg__CartesianCommand * data = NULL;

  if (size) {
    data = (so101_msgs__msg__CartesianCommand *)allocator.zero_allocate(size, sizeof(so101_msgs__msg__CartesianCommand), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = so101_msgs__msg__CartesianCommand__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        so101_msgs__msg__CartesianCommand__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
so101_msgs__msg__CartesianCommand__Sequence__fini(so101_msgs__msg__CartesianCommand__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      so101_msgs__msg__CartesianCommand__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

so101_msgs__msg__CartesianCommand__Sequence *
so101_msgs__msg__CartesianCommand__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  so101_msgs__msg__CartesianCommand__Sequence * array = (so101_msgs__msg__CartesianCommand__Sequence *)allocator.allocate(sizeof(so101_msgs__msg__CartesianCommand__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = so101_msgs__msg__CartesianCommand__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
so101_msgs__msg__CartesianCommand__Sequence__destroy(so101_msgs__msg__CartesianCommand__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    so101_msgs__msg__CartesianCommand__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
so101_msgs__msg__CartesianCommand__Sequence__are_equal(const so101_msgs__msg__CartesianCommand__Sequence * lhs, const so101_msgs__msg__CartesianCommand__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!so101_msgs__msg__CartesianCommand__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
so101_msgs__msg__CartesianCommand__Sequence__copy(
  const so101_msgs__msg__CartesianCommand__Sequence * input,
  so101_msgs__msg__CartesianCommand__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(so101_msgs__msg__CartesianCommand);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    so101_msgs__msg__CartesianCommand * data =
      (so101_msgs__msg__CartesianCommand *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!so101_msgs__msg__CartesianCommand__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          so101_msgs__msg__CartesianCommand__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!so101_msgs__msg__CartesianCommand__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
