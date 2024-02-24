// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from custom_interfaces:srv/ControlMovement.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__SRV__DETAIL__CONTROL_MOVEMENT__TRAITS_HPP_
#define CUSTOM_INTERFACES__SRV__DETAIL__CONTROL_MOVEMENT__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "custom_interfaces/srv/detail/control_movement__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace custom_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const ControlMovement_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: velocity
  {
    out << "velocity: ";
    rosidl_generator_traits::value_to_yaml(msg.velocity, out);
    out << ", ";
  }

  // member: radian
  {
    out << "radian: ";
    rosidl_generator_traits::value_to_yaml(msg.radian, out);
    out << ", ";
  }

  // member: delay
  {
    out << "delay: ";
    rosidl_generator_traits::value_to_yaml(msg.delay, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ControlMovement_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: velocity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "velocity: ";
    rosidl_generator_traits::value_to_yaml(msg.velocity, out);
    out << "\n";
  }

  // member: radian
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "radian: ";
    rosidl_generator_traits::value_to_yaml(msg.radian, out);
    out << "\n";
  }

  // member: delay
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "delay: ";
    rosidl_generator_traits::value_to_yaml(msg.delay, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ControlMovement_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace custom_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use custom_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const custom_interfaces::srv::ControlMovement_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  custom_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use custom_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const custom_interfaces::srv::ControlMovement_Request & msg)
{
  return custom_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<custom_interfaces::srv::ControlMovement_Request>()
{
  return "custom_interfaces::srv::ControlMovement_Request";
}

template<>
inline const char * name<custom_interfaces::srv::ControlMovement_Request>()
{
  return "custom_interfaces/srv/ControlMovement_Request";
}

template<>
struct has_fixed_size<custom_interfaces::srv::ControlMovement_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<custom_interfaces::srv::ControlMovement_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<custom_interfaces::srv::ControlMovement_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace custom_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const ControlMovement_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: status
  {
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ControlMovement_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: status
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ControlMovement_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace custom_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use custom_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const custom_interfaces::srv::ControlMovement_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  custom_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use custom_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const custom_interfaces::srv::ControlMovement_Response & msg)
{
  return custom_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<custom_interfaces::srv::ControlMovement_Response>()
{
  return "custom_interfaces::srv::ControlMovement_Response";
}

template<>
inline const char * name<custom_interfaces::srv::ControlMovement_Response>()
{
  return "custom_interfaces/srv/ControlMovement_Response";
}

template<>
struct has_fixed_size<custom_interfaces::srv::ControlMovement_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<custom_interfaces::srv::ControlMovement_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<custom_interfaces::srv::ControlMovement_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<custom_interfaces::srv::ControlMovement>()
{
  return "custom_interfaces::srv::ControlMovement";
}

template<>
inline const char * name<custom_interfaces::srv::ControlMovement>()
{
  return "custom_interfaces/srv/ControlMovement";
}

template<>
struct has_fixed_size<custom_interfaces::srv::ControlMovement>
  : std::integral_constant<
    bool,
    has_fixed_size<custom_interfaces::srv::ControlMovement_Request>::value &&
    has_fixed_size<custom_interfaces::srv::ControlMovement_Response>::value
  >
{
};

template<>
struct has_bounded_size<custom_interfaces::srv::ControlMovement>
  : std::integral_constant<
    bool,
    has_bounded_size<custom_interfaces::srv::ControlMovement_Request>::value &&
    has_bounded_size<custom_interfaces::srv::ControlMovement_Response>::value
  >
{
};

template<>
struct is_service<custom_interfaces::srv::ControlMovement>
  : std::true_type
{
};

template<>
struct is_service_request<custom_interfaces::srv::ControlMovement_Request>
  : std::true_type
{
};

template<>
struct is_service_response<custom_interfaces::srv::ControlMovement_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // CUSTOM_INTERFACES__SRV__DETAIL__CONTROL_MOVEMENT__TRAITS_HPP_
