// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from so101_msgs:msg/JointVelocityCommand.idl
// generated code does not contain a copyright notice
#include "so101_msgs/msg/detail/joint_velocity_command__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `joint_names`
#include "rosidl_runtime_c/string_functions.h"
// Member `velocities`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
so101_msgs__msg__JointVelocityCommand__init(so101_msgs__msg__JointVelocityCommand * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    so101_msgs__msg__JointVelocityCommand__fini(msg);
    return false;
  }
  // joint_names
  if (!rosidl_runtime_c__String__Sequence__init(&msg->joint_names, 0)) {
    so101_msgs__msg__JointVelocityCommand__fini(msg);
    return false;
  }
  // velocities
  if (!rosidl_runtime_c__double__Sequence__init(&msg->velocities, 0)) {
    so101_msgs__msg__JointVelocityCommand__fini(msg);
    return false;
  }
  return true;
}

void
so101_msgs__msg__JointVelocityCommand__fini(so101_msgs__msg__JointVelocityCommand * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // joint_names
  rosidl_runtime_c__String__Sequence__fini(&msg->joint_names);
  // velocities
  rosidl_runtime_c__double__Sequence__fini(&msg->velocities);
}

bool
so101_msgs__msg__JointVelocityCommand__are_equal(const so101_msgs__msg__JointVelocityCommand * lhs, const so101_msgs__msg__JointVelocityCommand * rhs)
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
  // joint_names
  if (!rosidl_runtime_c__String__Sequence__are_equal(
      &(lhs->joint_names), &(rhs->joint_names)))
  {
    return false;
  }
  // velocities
  if (!rosidl_runtime_c__double__Sequence__are_equal(
      &(lhs->velocities), &(rhs->velocities)))
  {
    return false;
  }
  return true;
}

bool
so101_msgs__msg__JointVelocityCommand__copy(
  const so101_msgs__msg__JointVelocityCommand * input,
  so101_msgs__msg__JointVelocityCommand * output)
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
  // joint_names
  if (!rosidl_runtime_c__String__Sequence__copy(
      &(input->joint_names), &(output->joint_names)))
  {
    return false;
  }
  // velocities
  if (!rosidl_runtime_c__double__Sequence__copy(
      &(input->velocities), &(output->velocities)))
  {
    return false;
  }
  return true;
}

so101_msgs__msg__JointVelocityCommand *
so101_msgs__msg__JointVelocityCommand__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  so101_msgs__msg__JointVelocityCommand * msg = (so101_msgs__msg__JointVelocityCommand *)allocator.allocate(sizeof(so101_msgs__msg__JointVelocityCommand), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(so101_msgs__msg__JointVelocityCommand));
  bool success = so101_msgs__msg__JointVelocityCommand__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
so101_msgs__msg__JointVelocityCommand__destroy(so101_msgs__msg__JointVelocityCommand * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    so101_msgs__msg__JointVelocityCommand__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
so101_msgs__msg__JointVelocityCommand__Sequence__init(so101_msgs__msg__JointVelocityCommand__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  so101_msgs__msg__JointVelocityCommand * data = NULL;

  if (size) {
    data = (so101_msgs__msg__JointVelocityCommand *)allocator.zero_allocate(size, sizeof(so101_msgs__msg__JointVelocityCommand), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = so101_msgs__msg__JointVelocityCommand__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        so101_msgs__msg__JointVelocityCommand__fini(&data[i - 1]);
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
so101_msgs__msg__JointVelocityCommand__Sequence__fini(so101_msgs__msg__JointVelocityCommand__Sequence * array)
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
      so101_msgs__msg__JointVelocityCommand__fini(&array->data[i]);
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

so101_msgs__msg__JointVelocityCommand__Sequence *
so101_msgs__msg__JointVelocityCommand__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  so101_msgs__msg__JointVelocityCommand__Sequence * array = (so101_msgs__msg__JointVelocityCommand__Sequence *)allocator.allocate(sizeof(so101_msgs__msg__JointVelocityCommand__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = so101_msgs__msg__JointVelocityCommand__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
so101_msgs__msg__JointVelocityCommand__Sequence__destroy(so101_msgs__msg__JointVelocityCommand__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    so101_msgs__msg__JointVelocityCommand__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
so101_msgs__msg__JointVelocityCommand__Sequence__are_equal(const so101_msgs__msg__JointVelocityCommand__Sequence * lhs, const so101_msgs__msg__JointVelocityCommand__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!so101_msgs__msg__JointVelocityCommand__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
so101_msgs__msg__JointVelocityCommand__Sequence__copy(
  const so101_msgs__msg__JointVelocityCommand__Sequence * input,
  so101_msgs__msg__JointVelocityCommand__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(so101_msgs__msg__JointVelocityCommand);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    so101_msgs__msg__JointVelocityCommand * data =
      (so101_msgs__msg__JointVelocityCommand *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!so101_msgs__msg__JointVelocityCommand__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          so101_msgs__msg__JointVelocityCommand__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!so101_msgs__msg__JointVelocityCommand__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
