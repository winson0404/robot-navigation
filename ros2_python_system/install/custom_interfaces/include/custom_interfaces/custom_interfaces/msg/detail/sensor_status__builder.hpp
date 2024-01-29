// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_interfaces:msg/SensorStatus.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__BUILDER_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_interfaces/msg/detail/sensor_status__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_SensorStatus_front_us
{
public:
  explicit Init_SensorStatus_front_us(::custom_interfaces::msg::SensorStatus & msg)
  : msg_(msg)
  {}
  ::custom_interfaces::msg::SensorStatus front_us(::custom_interfaces::msg::SensorStatus::_front_us_type arg)
  {
    msg_.front_us = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::msg::SensorStatus msg_;
};

class Init_SensorStatus_front_ir
{
public:
  explicit Init_SensorStatus_front_ir(::custom_interfaces::msg::SensorStatus & msg)
  : msg_(msg)
  {}
  Init_SensorStatus_front_us front_ir(::custom_interfaces::msg::SensorStatus::_front_ir_type arg)
  {
    msg_.front_ir = std::move(arg);
    return Init_SensorStatus_front_us(msg_);
  }

private:
  ::custom_interfaces::msg::SensorStatus msg_;
};

class Init_SensorStatus_right_ir
{
public:
  explicit Init_SensorStatus_right_ir(::custom_interfaces::msg::SensorStatus & msg)
  : msg_(msg)
  {}
  Init_SensorStatus_front_ir right_ir(::custom_interfaces::msg::SensorStatus::_right_ir_type arg)
  {
    msg_.right_ir = std::move(arg);
    return Init_SensorStatus_front_ir(msg_);
  }

private:
  ::custom_interfaces::msg::SensorStatus msg_;
};

class Init_SensorStatus_left_ir
{
public:
  Init_SensorStatus_left_ir()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SensorStatus_right_ir left_ir(::custom_interfaces::msg::SensorStatus::_left_ir_type arg)
  {
    msg_.left_ir = std::move(arg);
    return Init_SensorStatus_right_ir(msg_);
  }

private:
  ::custom_interfaces::msg::SensorStatus msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::msg::SensorStatus>()
{
  return custom_interfaces::msg::builder::Init_SensorStatus_left_ir();
}

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__BUILDER_HPP_
