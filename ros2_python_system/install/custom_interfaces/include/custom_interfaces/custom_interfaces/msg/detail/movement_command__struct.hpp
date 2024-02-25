// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from custom_interfaces:msg/MovementCommand.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__MSG__DETAIL__MOVEMENT_COMMAND__STRUCT_HPP_
#define CUSTOM_INTERFACES__MSG__DETAIL__MOVEMENT_COMMAND__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__custom_interfaces__msg__MovementCommand __attribute__((deprecated))
#else
# define DEPRECATED__custom_interfaces__msg__MovementCommand __declspec(deprecated)
#endif

namespace custom_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct MovementCommand_
{
  using Type = MovementCommand_<ContainerAllocator>;

  explicit MovementCommand_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->velocity = 0.0f;
      this->radian = 0.0f;
      this->delay = 0l;
    }
  }

  explicit MovementCommand_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->velocity = 0.0f;
      this->radian = 0.0f;
      this->delay = 0l;
    }
  }

  // field types and members
  using _velocity_type =
    float;
  _velocity_type velocity;
  using _radian_type =
    float;
  _radian_type radian;
  using _delay_type =
    int32_t;
  _delay_type delay;

  // setters for named parameter idiom
  Type & set__velocity(
    const float & _arg)
  {
    this->velocity = _arg;
    return *this;
  }
  Type & set__radian(
    const float & _arg)
  {
    this->radian = _arg;
    return *this;
  }
  Type & set__delay(
    const int32_t & _arg)
  {
    this->delay = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    custom_interfaces::msg::MovementCommand_<ContainerAllocator> *;
  using ConstRawPtr =
    const custom_interfaces::msg::MovementCommand_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<custom_interfaces::msg::MovementCommand_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<custom_interfaces::msg::MovementCommand_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::msg::MovementCommand_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::msg::MovementCommand_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::msg::MovementCommand_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::msg::MovementCommand_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<custom_interfaces::msg::MovementCommand_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<custom_interfaces::msg::MovementCommand_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__custom_interfaces__msg__MovementCommand
    std::shared_ptr<custom_interfaces::msg::MovementCommand_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__custom_interfaces__msg__MovementCommand
    std::shared_ptr<custom_interfaces::msg::MovementCommand_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MovementCommand_ & other) const
  {
    if (this->velocity != other.velocity) {
      return false;
    }
    if (this->radian != other.radian) {
      return false;
    }
    if (this->delay != other.delay) {
      return false;
    }
    return true;
  }
  bool operator!=(const MovementCommand_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MovementCommand_

// alias to use template instance with default allocator
using MovementCommand =
  custom_interfaces::msg::MovementCommand_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__MSG__DETAIL__MOVEMENT_COMMAND__STRUCT_HPP_
