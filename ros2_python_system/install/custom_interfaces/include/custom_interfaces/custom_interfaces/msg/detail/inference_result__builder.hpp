// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_interfaces:msg/InferenceResult.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__INFERENCE_RESULT__BUILDER_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__INFERENCE_RESULT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_interfaces/msg/detail/inference_result__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_InferenceResult_model_result
{
public:
  Init_InferenceResult_model_result()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::custom_interfaces::msg::InferenceResult model_result(::custom_interfaces::msg::InferenceResult::_model_result_type arg)
  {
    msg_.model_result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::msg::InferenceResult msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::msg::InferenceResult>()
{
  return custom_interfaces::msg::builder::Init_InferenceResult_model_result();
}

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__INFERENCE_RESULT__BUILDER_HPP_
