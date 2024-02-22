// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from custom_interfaces:msg/InferenceResult.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__INFERENCE_RESULT__STRUCT_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__INFERENCE_RESULT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__custom_interfaces__msg__InferenceResult __attribute__((deprecated))
#else
# define DEPRECATED__custom_interfaces__msg__InferenceResult __declspec(deprecated)
#endif

namespace custom_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct InferenceResult_
{
  using Type = InferenceResult_<ContainerAllocator>;

  explicit InferenceResult_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->model_result = 0;
    }
  }

  explicit InferenceResult_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->model_result = 0;
    }
  }

  // field types and members
  using _model_result_type =
    int8_t;
  _model_result_type model_result;

  // setters for named parameter idiom
  Type & set__model_result(
    const int8_t & _arg)
  {
    this->model_result = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    custom_interfaces::msg::InferenceResult_<ContainerAllocator> *;
  using ConstRawPtr =
    const custom_interfaces::msg::InferenceResult_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<custom_interfaces::msg::InferenceResult_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<custom_interfaces::msg::InferenceResult_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::msg::InferenceResult_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::msg::InferenceResult_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::msg::InferenceResult_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::msg::InferenceResult_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<custom_interfaces::msg::InferenceResult_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<custom_interfaces::msg::InferenceResult_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__custom_interfaces__msg__InferenceResult
    std::shared_ptr<custom_interfaces::msg::InferenceResult_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__custom_interfaces__msg__InferenceResult
    std::shared_ptr<custom_interfaces::msg::InferenceResult_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const InferenceResult_ & other) const
  {
    if (this->model_result != other.model_result) {
      return false;
    }
    return true;
  }
  bool operator!=(const InferenceResult_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct InferenceResult_

// alias to use template instance with default allocator
using InferenceResult =
  custom_interfaces::msg::InferenceResult_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__INFERENCE_RESULT__STRUCT_HPP_
