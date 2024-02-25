// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_interfaces:msg/MovementCommand.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__MOVEMENT_COMMAND__BUILDER_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__MOVEMENT_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_interfaces/msg/detail/movement_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_MovementCommand_delay
{
public:
  explicit Init_MovementCommand_delay(::custom_interfaces::msg::MovementCommand & msg)
  : msg_(msg)
  {}
  ::custom_interfaces::msg::MovementCommand delay(::custom_interfaces::msg::MovementCommand::_delay_type arg)
  {
    msg_.delay = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::msg::MovementCommand msg_;
};

class Init_MovementCommand_radian
{
public:
  explicit Init_MovementCommand_radian(::custom_interfaces::msg::MovementCommand & msg)
  : msg_(msg)
  {}
  Init_MovementCommand_delay radian(::custom_interfaces::msg::MovementCommand::_radian_type arg)
  {
    msg_.radian = std::move(arg);
    return Init_MovementCommand_delay(msg_);
  }

private:
  ::custom_interfaces::msg::MovementCommand msg_;
};

class Init_MovementCommand_velocity
{
public:
  Init_MovementCommand_velocity()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MovementCommand_radian velocity(::custom_interfaces::msg::MovementCommand::_velocity_type arg)
  {
    msg_.velocity = std::move(arg);
    return Init_MovementCommand_radian(msg_);
  }

private:
  ::custom_interfaces::msg::MovementCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::msg::MovementCommand>()
{
  return custom_interfaces::msg::builder::Init_MovementCommand_velocity();
}

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__MOVEMENT_COMMAND__BUILDER_HPP_
