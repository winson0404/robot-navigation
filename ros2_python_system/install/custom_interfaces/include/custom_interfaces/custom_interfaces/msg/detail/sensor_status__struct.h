// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from custom_interfaces:msg/SensorStatus.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__STRUCT_H_
#define CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/SensorStatus in the package custom_interfaces.
typedef struct custom_interfaces__msg__SensorStatus
{
  int8_t left_ir;
  int8_t right_ir;
  int8_t front_ir;
  float front_us;
} custom_interfaces__msg__SensorStatus;

// Struct for a sequence of custom_interfaces__msg__SensorStatus.
typedef struct custom_interfaces__msg__SensorStatus__Sequence
{
  custom_interfaces__msg__SensorStatus * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} custom_interfaces__msg__SensorStatus__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__STRUCT_H_
