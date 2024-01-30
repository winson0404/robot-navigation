// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from custom_interfaces:srv/ControlMovement.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__SRV__DETAIL__CONTROL_MOVEMENT__STRUCT_HPP_
#define CUSTOM_INTERFACES__SRV__DETAIL__CONTROL_MOVEMENT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__custom_interfaces__srv__ControlMovement_Request __attribute__((deprecated))
#else
# define DEPRECATED__custom_interfaces__srv__ControlMovement_Request __declspec(deprecated)
#endif

namespace custom_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ControlMovement_Request_
{
  using Type = ControlMovement_Request_<ContainerAllocator>;

  explicit ControlMovement_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->velocity = 0.0f;
      this->radian = 0.0f;
    }
  }

  explicit ControlMovement_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->velocity = 0.0f;
      this->radian = 0.0f;
    }
  }

  // field types and members
  using _velocity_type =
    float;
  _velocity_type velocity;
  using _radian_type =
    float;
  _radian_type radian;

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

  // constant declarations

  // pointer types
  using RawPtr =
    custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__custom_interfaces__srv__ControlMovement_Request
    std::shared_ptr<custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__custom_interfaces__srv__ControlMovement_Request
    std::shared_ptr<custom_interfaces::srv::ControlMovement_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ControlMovement_Request_ & other) const
  {
    if (this->velocity != other.velocity) {
      return false;
    }
    if (this->radian != other.radian) {
      return false;
    }
    return true;
  }
  bool operator!=(const ControlMovement_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ControlMovement_Request_

// alias to use template instance with default allocator
using ControlMovement_Request =
  custom_interfaces::srv::ControlMovement_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace custom_interfaces


#ifndef _WIN32
# define DEPRECATED__custom_interfaces__srv__ControlMovement_Response __attribute__((deprecated))
#else
# define DEPRECATED__custom_interfaces__srv__ControlMovement_Response __declspec(deprecated)
#endif

namespace custom_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ControlMovement_Response_
{
  using Type = ControlMovement_Response_<ContainerAllocator>;

  explicit ControlMovement_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->status = false;
    }
  }

  explicit ControlMovement_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->status = false;
    }
  }

  // field types and members
  using _status_type =
    bool;
  _status_type status;

  // setters for named parameter idiom
  Type & set__status(
    const bool & _arg)
  {
    this->status = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__custom_interfaces__srv__ControlMovement_Response
    std::shared_ptr<custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__custom_interfaces__srv__ControlMovement_Response
    std::shared_ptr<custom_interfaces::srv::ControlMovement_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ControlMovement_Response_ & other) const
  {
    if (this->status != other.status) {
      return false;
    }
    return true;
  }
  bool operator!=(const ControlMovement_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ControlMovement_Response_

// alias to use template instance with default allocator
using ControlMovement_Response =
  custom_interfaces::srv::ControlMovement_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace custom_interfaces

namespace custom_interfaces
{

namespace srv
{

struct ControlMovement
{
  using Request = custom_interfaces::srv::ControlMovement_Request;
  using Response = custom_interfaces::srv::ControlMovement_Response;
};

}  // namespace srv

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__SRV__DETAIL__CONTROL_MOVEMENT__STRUCT_HPP_
