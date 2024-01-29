// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from custom_interfaces:msg/SensorStatus.idl
// generated code does not contain a copyright notice
#include "custom_interfaces/msg/detail/sensor_status__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
custom_interfaces__msg__SensorStatus__init(custom_interfaces__msg__SensorStatus * msg)
{
  if (!msg) {
    return false;
  }
  // left_ir
  // right_ir
  // front_ir
  // front_us
  return true;
}

void
custom_interfaces__msg__SensorStatus__fini(custom_interfaces__msg__SensorStatus * msg)
{
  if (!msg) {
    return;
  }
  // left_ir
  // right_ir
  // front_ir
  // front_us
}

bool
custom_interfaces__msg__SensorStatus__are_equal(const custom_interfaces__msg__SensorStatus * lhs, const custom_interfaces__msg__SensorStatus * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // left_ir
  if (lhs->left_ir != rhs->left_ir) {
    return false;
  }
  // right_ir
  if (lhs->right_ir != rhs->right_ir) {
    return false;
  }
  // front_ir
  if (lhs->front_ir != rhs->front_ir) {
    return false;
  }
  // front_us
  if (lhs->front_us != rhs->front_us) {
    return false;
  }
  return true;
}

bool
custom_interfaces__msg__SensorStatus__copy(
  const custom_interfaces__msg__SensorStatus * input,
  custom_interfaces__msg__SensorStatus * output)
{
  if (!input || !output) {
    return false;
  }
  // left_ir
  output->left_ir = input->left_ir;
  // right_ir
  output->right_ir = input->right_ir;
  // front_ir
  output->front_ir = input->front_ir;
  // front_us
  output->front_us = input->front_us;
  return true;
}

custom_interfaces__msg__SensorStatus *
custom_interfaces__msg__SensorStatus__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  custom_interfaces__msg__SensorStatus * msg = (custom_interfaces__msg__SensorStatus *)allocator.allocate(sizeof(custom_interfaces__msg__SensorStatus), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(custom_interfaces__msg__SensorStatus));
  bool success = custom_interfaces__msg__SensorStatus__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
custom_interfaces__msg__SensorStatus__destroy(custom_interfaces__msg__SensorStatus * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    custom_interfaces__msg__SensorStatus__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
custom_interfaces__msg__SensorStatus__Sequence__init(custom_interfaces__msg__SensorStatus__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  custom_interfaces__msg__SensorStatus * data = NULL;

  if (size) {
    data = (custom_interfaces__msg__SensorStatus *)allocator.zero_allocate(size, sizeof(custom_interfaces__msg__SensorStatus), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = custom_interfaces__msg__SensorStatus__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        custom_interfaces__msg__SensorStatus__fini(&data[i - 1]);
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
custom_interfaces__msg__SensorStatus__Sequence__fini(custom_interfaces__msg__SensorStatus__Sequence * array)
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
      custom_interfaces__msg__SensorStatus__fini(&array->data[i]);
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

custom_interfaces__msg__SensorStatus__Sequence *
custom_interfaces__msg__SensorStatus__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  custom_interfaces__msg__SensorStatus__Sequence * array = (custom_interfaces__msg__SensorStatus__Sequence *)allocator.allocate(sizeof(custom_interfaces__msg__SensorStatus__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = custom_interfaces__msg__SensorStatus__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
custom_interfaces__msg__SensorStatus__Sequence__destroy(custom_interfaces__msg__SensorStatus__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    custom_interfaces__msg__SensorStatus__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
custom_interfaces__msg__SensorStatus__Sequence__are_equal(const custom_interfaces__msg__SensorStatus__Sequence * lhs, const custom_interfaces__msg__SensorStatus__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!custom_interfaces__msg__SensorStatus__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
custom_interfaces__msg__SensorStatus__Sequence__copy(
  const custom_interfaces__msg__SensorStatus__Sequence * input,
  custom_interfaces__msg__SensorStatus__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(custom_interfaces__msg__SensorStatus);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    custom_interfaces__msg__SensorStatus * data =
      (custom_interfaces__msg__SensorStatus *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!custom_interfaces__msg__SensorStatus__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          custom_interfaces__msg__SensorStatus__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!custom_interfaces__msg__SensorStatus__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
