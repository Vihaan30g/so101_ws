// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from so101_msgs:msg/ControllerStatus.idl
// generated code does not contain a copyright notice
#include "so101_msgs/msg/detail/controller_status__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `last_error`
#include "rosidl_runtime_c/string_functions.h"

bool
so101_msgs__msg__ControllerStatus__init(so101_msgs__msg__ControllerStatus * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    so101_msgs__msg__ControllerStatus__fini(msg);
    return false;
  }
  // in_singularity
  // manipulability_index
  // near_joint_limit
  // self_collision_detected
  // last_error
  if (!rosidl_runtime_c__String__init(&msg->last_error)) {
    so101_msgs__msg__ControllerStatus__fini(msg);
    return false;
  }
  return true;
}

void
so101_msgs__msg__ControllerStatus__fini(so101_msgs__msg__ControllerStatus * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // in_singularity
  // manipulability_index
  // near_joint_limit
  // self_collision_detected
  // last_error
  rosidl_runtime_c__String__fini(&msg->last_error);
}

bool
so101_msgs__msg__ControllerStatus__are_equal(const so101_msgs__msg__ControllerStatus * lhs, const so101_msgs__msg__ControllerStatus * rhs)
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
  // in_singularity
  if (lhs->in_singularity != rhs->in_singularity) {
    return false;
  }
  // manipulability_index
  if (lhs->manipulability_index != rhs->manipulability_index) {
    return false;
  }
  // near_joint_limit
  if (lhs->near_joint_limit != rhs->near_joint_limit) {
    return false;
  }
  // self_collision_detected
  if (lhs->self_collision_detected != rhs->self_collision_detected) {
    return false;
  }
  // last_error
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->last_error), &(rhs->last_error)))
  {
    return false;
  }
  return true;
}

bool
so101_msgs__msg__ControllerStatus__copy(
  const so101_msgs__msg__ControllerStatus * input,
  so101_msgs__msg__ControllerStatus * output)
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
  // in_singularity
  output->in_singularity = input->in_singularity;
  // manipulability_index
  output->manipulability_index = input->manipulability_index;
  // near_joint_limit
  output->near_joint_limit = input->near_joint_limit;
  // self_collision_detected
  output->self_collision_detected = input->self_collision_detected;
  // last_error
  if (!rosidl_runtime_c__String__copy(
      &(input->last_error), &(output->last_error)))
  {
    return false;
  }
  return true;
}

so101_msgs__msg__ControllerStatus *
so101_msgs__msg__ControllerStatus__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  so101_msgs__msg__ControllerStatus * msg = (so101_msgs__msg__ControllerStatus *)allocator.allocate(sizeof(so101_msgs__msg__ControllerStatus), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(so101_msgs__msg__ControllerStatus));
  bool success = so101_msgs__msg__ControllerStatus__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
so101_msgs__msg__ControllerStatus__destroy(so101_msgs__msg__ControllerStatus * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    so101_msgs__msg__ControllerStatus__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
so101_msgs__msg__ControllerStatus__Sequence__init(so101_msgs__msg__ControllerStatus__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  so101_msgs__msg__ControllerStatus * data = NULL;

  if (size) {
    data = (so101_msgs__msg__ControllerStatus *)allocator.zero_allocate(size, sizeof(so101_msgs__msg__ControllerStatus), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = so101_msgs__msg__ControllerStatus__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        so101_msgs__msg__ControllerStatus__fini(&data[i - 1]);
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
so101_msgs__msg__ControllerStatus__Sequence__fini(so101_msgs__msg__ControllerStatus__Sequence * array)
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
      so101_msgs__msg__ControllerStatus__fini(&array->data[i]);
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

so101_msgs__msg__ControllerStatus__Sequence *
so101_msgs__msg__ControllerStatus__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  so101_msgs__msg__ControllerStatus__Sequence * array = (so101_msgs__msg__ControllerStatus__Sequence *)allocator.allocate(sizeof(so101_msgs__msg__ControllerStatus__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = so101_msgs__msg__ControllerStatus__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
so101_msgs__msg__ControllerStatus__Sequence__destroy(so101_msgs__msg__ControllerStatus__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    so101_msgs__msg__ControllerStatus__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
so101_msgs__msg__ControllerStatus__Sequence__are_equal(const so101_msgs__msg__ControllerStatus__Sequence * lhs, const so101_msgs__msg__ControllerStatus__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!so101_msgs__msg__ControllerStatus__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
so101_msgs__msg__ControllerStatus__Sequence__copy(
  const so101_msgs__msg__ControllerStatus__Sequence * input,
  so101_msgs__msg__ControllerStatus__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(so101_msgs__msg__ControllerStatus);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    so101_msgs__msg__ControllerStatus * data =
      (so101_msgs__msg__ControllerStatus *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!so101_msgs__msg__ControllerStatus__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          so101_msgs__msg__ControllerStatus__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!so101_msgs__msg__ControllerStatus__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
