// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_interfaces:srv/ControlMovement.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__SRV__DETAIL__CONTROL_MOVEMENT__BUILDER_HPP_
#define CUSTOM_INTERFACES__SRV__DETAIL__CONTROL_MOVEMENT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_interfaces/srv/detail/control_movement__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_interfaces
{

namespace srv
{

namespace builder
{

class Init_ControlMovement_Request_delay
{
public:
  explicit Init_ControlMovement_Request_delay(::custom_interfaces::srv::ControlMovement_Request & msg)
  : msg_(msg)
  {}
  ::custom_interfaces::srv::ControlMovement_Request delay(::custom_interfaces::srv::ControlMovement_Request::_delay_type arg)
  {
    msg_.delay = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::srv::ControlMovement_Request msg_;
};

class Init_ControlMovement_Request_radian
{
public:
  explicit Init_ControlMovement_Request_radian(::custom_interfaces::srv::ControlMovement_Request & msg)
  : msg_(msg)
  {}
  Init_ControlMovement_Request_delay radian(::custom_interfaces::srv::ControlMovement_Request::_radian_type arg)
  {
    msg_.radian = std::move(arg);
    return Init_ControlMovement_Request_delay(msg_);
  }

private:
  ::custom_interfaces::srv::ControlMovement_Request msg_;
};

class Init_ControlMovement_Request_velocity
{
public:
  Init_ControlMovement_Request_velocity()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ControlMovement_Request_radian velocity(::custom_interfaces::srv::ControlMovement_Request::_velocity_type arg)
  {
    msg_.velocity = std::move(arg);
    return Init_ControlMovement_Request_radian(msg_);
  }

private:
  ::custom_interfaces::srv::ControlMovement_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::srv::ControlMovement_Request>()
{
  return custom_interfaces::srv::builder::Init_ControlMovement_Request_velocity();
}

}  // namespace custom_interfaces


namespace custom_interfaces
{

namespace srv
{

namespace builder
{

class Init_ControlMovement_Response_status
{
public:
  Init_ControlMovement_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::custom_interfaces::srv::ControlMovement_Response status(::custom_interfaces::srv::ControlMovement_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::srv::ControlMovement_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::srv::ControlMovement_Response>()
{
  return custom_interfaces::srv::builder::Init_ControlMovement_Response_status();
}

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__SRV__DETAIL__CONTROL_MOVEMENT__BUILDER_HPP_
