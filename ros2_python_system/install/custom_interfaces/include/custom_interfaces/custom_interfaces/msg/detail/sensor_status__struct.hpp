// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from custom_interfaces:msg/SensorStatus.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__STRUCT_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__custom_interfaces__msg__SensorStatus __attribute__((deprecated))
#else
# define DEPRECATED__custom_interfaces__msg__SensorStatus __declspec(deprecated)
#endif

namespace custom_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SensorStatus_
{
  using Type = SensorStatus_<ContainerAllocator>;

  explicit SensorStatus_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->left_ir = 0.0f;
      this->right_ir = 0.0f;
      this->front_ir = 0.0f;
      this->front_us = 0.0f;
    }
  }

  explicit SensorStatus_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->left_ir = 0.0f;
      this->right_ir = 0.0f;
      this->front_ir = 0.0f;
      this->front_us = 0.0f;
    }
  }

  // field types and members
  using _left_ir_type =
    float;
  _left_ir_type left_ir;
  using _right_ir_type =
    float;
  _right_ir_type right_ir;
  using _front_ir_type =
    float;
  _front_ir_type front_ir;
  using _front_us_type =
    float;
  _front_us_type front_us;

  // setters for named parameter idiom
  Type & set__left_ir(
    const float & _arg)
  {
    this->left_ir = _arg;
    return *this;
  }
  Type & set__right_ir(
    const float & _arg)
  {
    this->right_ir = _arg;
    return *this;
  }
  Type & set__front_ir(
    const float & _arg)
  {
    this->front_ir = _arg;
    return *this;
  }
  Type & set__front_us(
    const float & _arg)
  {
    this->front_us = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    custom_interfaces::msg::SensorStatus_<ContainerAllocator> *;
  using ConstRawPtr =
    const custom_interfaces::msg::SensorStatus_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<custom_interfaces::msg::SensorStatus_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<custom_interfaces::msg::SensorStatus_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::msg::SensorStatus_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::msg::SensorStatus_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::msg::SensorStatus_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::msg::SensorStatus_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<custom_interfaces::msg::SensorStatus_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<custom_interfaces::msg::SensorStatus_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__custom_interfaces__msg__SensorStatus
    std::shared_ptr<custom_interfaces::msg::SensorStatus_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__custom_interfaces__msg__SensorStatus
    std::shared_ptr<custom_interfaces::msg::SensorStatus_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SensorStatus_ & other) const
  {
    if (this->left_ir != other.left_ir) {
      return false;
    }
    if (this->right_ir != other.right_ir) {
      return false;
    }
    if (this->front_ir != other.front_ir) {
      return false;
    }
    if (this->front_us != other.front_us) {
      return false;
    }
    return true;
  }
  bool operator!=(const SensorStatus_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SensorStatus_

// alias to use template instance with default allocator
using SensorStatus =
  custom_interfaces::msg::SensorStatus_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__SENSOR_STATUS__STRUCT_HPP_
