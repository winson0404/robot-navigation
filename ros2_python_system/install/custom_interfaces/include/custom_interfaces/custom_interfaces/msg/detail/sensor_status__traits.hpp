// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from custom_interfaces:msg/SensorStatus.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__TRAITS_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "custom_interfaces/msg/detail/sensor_status__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace custom_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const SensorStatus & msg,
  std::ostream & out)
{
  out << "{";
  // member: left_ir
  {
    out << "left_ir: ";
    rosidl_generator_traits::value_to_yaml(msg.left_ir, out);
    out << ", ";
  }

  // member: right_ir
  {
    out << "right_ir: ";
    rosidl_generator_traits::value_to_yaml(msg.right_ir, out);
    out << ", ";
  }

  // member: front_ir
  {
    out << "front_ir: ";
    rosidl_generator_traits::value_to_yaml(msg.front_ir, out);
    out << ", ";
  }

  // member: front_us
  {
    out << "front_us: ";
    rosidl_generator_traits::value_to_yaml(msg.front_us, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SensorStatus & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: left_ir
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "left_ir: ";
    rosidl_generator_traits::value_to_yaml(msg.left_ir, out);
    out << "\n";
  }

  // member: right_ir
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "right_ir: ";
    rosidl_generator_traits::value_to_yaml(msg.right_ir, out);
    out << "\n";
  }

  // member: front_ir
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "front_ir: ";
    rosidl_generator_traits::value_to_yaml(msg.front_ir, out);
    out << "\n";
  }

  // member: front_us
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "front_us: ";
    rosidl_generator_traits::value_to_yaml(msg.front_us, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SensorStatus & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace custom_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use custom_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const custom_interfaces::msg::SensorStatus & msg,
  std::ostream & out, size_t indentation = 0)
{
  custom_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use custom_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const custom_interfaces::msg::SensorStatus & msg)
{
  return custom_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<custom_interfaces::msg::SensorStatus>()
{
  return "custom_interfaces::msg::SensorStatus";
}

template<>
inline const char * name<custom_interfaces::msg::SensorStatus>()
{
  return "custom_interfaces/msg/SensorStatus";
}

template<>
struct has_fixed_size<custom_interfaces::msg::SensorStatus>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<custom_interfaces::msg::SensorStatus>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<custom_interfaces::msg::SensorStatus>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__TRAITS_HPP_
