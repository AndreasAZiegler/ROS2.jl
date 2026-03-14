#include <ros2_julia/messages.hpp>
#include <ros2_julia/utils.hpp>

#include <cstring>
#include <stdexcept>

#if __has_include(<std_msgs/msg/string.h>)
  #define ROS2_JULIA_HAS_STD_MSGS_STRING 1
  #include <std_msgs/msg/string.h>
  #include <rosidl_runtime_c/string_functions.h>
  #if __has_include(<rosidl_typesupport_c/message_type_support_dispatch.h>)
    #include <rosidl_typesupport_c/message_type_support_dispatch.h>
  #elif __has_include(<rosidl_typesupport_interface/macros.h>)
    #include <rosidl_typesupport_interface/macros.h>
  #endif
#else
  #define ROS2_JULIA_HAS_STD_MSGS_STRING 0
#endif

namespace ros2_julia
{

struct StdMsgsString::Impl
{
#if ROS2_JULIA_HAS_STD_MSGS_STRING
  std_msgs__msg__String msg;
  bool initialized = false;
#else
  // Placeholder to keep type complete even when std_msgs isn't installed.
  int dummy = 0;
#endif
};

StdMsgsString::StdMsgsString()
{
  impl_ = new Impl();
#if ROS2_JULIA_HAS_STD_MSGS_STRING
  if (!std_msgs__msg__String__init(&impl_->msg)) {
    delete impl_;
    impl_ = nullptr;
    throw std::runtime_error("std_msgs__msg__String__init failed");
  }
  impl_->initialized = true;
#else
  throw std::runtime_error("StdMsgsString unavailable: std_msgs/msg/string.h not found at build time");
#endif
}

StdMsgsString::StdMsgsString(const std::string& data)
  : StdMsgsString()
{
  set_data(data);
}

StdMsgsString::~StdMsgsString()
{
  if (impl_ == nullptr) {
    return;
  }
#if ROS2_JULIA_HAS_STD_MSGS_STRING
  if (impl_->initialized) {
    std_msgs__msg__String__fini(&impl_->msg);
  }
#endif
  delete impl_;
  impl_ = nullptr;
}

void StdMsgsString::set_data(const std::string& data)
{
#if ROS2_JULIA_HAS_STD_MSGS_STRING
  if (!rosidl_runtime_c__String__assign(&impl_->msg.data, data.c_str())) {
    throw std::runtime_error("rosidl_runtime_c__String__assign failed");
  }
#else
  (void)data;
  throw std::runtime_error("StdMsgsString unavailable");
#endif
}

std::string StdMsgsString::get_data() const
{
#if ROS2_JULIA_HAS_STD_MSGS_STRING
  if (impl_->msg.data.data == nullptr) {
    return std::string();
  }
  return std::string(impl_->msg.data.data);
#else
  return std::string();
#endif
}

void* StdMsgsString::ros_message()
{
#if ROS2_JULIA_HAS_STD_MSGS_STRING
  return static_cast<void*>(&impl_->msg);
#else
  return nullptr;
#endif
}

const void* StdMsgsString::ros_message() const
{
#if ROS2_JULIA_HAS_STD_MSGS_STRING
  return static_cast<const void*>(&impl_->msg);
#else
  return nullptr;
#endif
}

const void* typesupport_std_msgs_string()
{
#if ROS2_JULIA_HAS_STD_MSGS_STRING
  return static_cast<const void*>(ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String));
#else
  return nullptr;
#endif
}

}  // namespace ros2_julia
