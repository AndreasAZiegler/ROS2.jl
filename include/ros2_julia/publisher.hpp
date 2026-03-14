#pragma once

#include <ros2_julia/node.hpp>

#include <rcl/publisher.h>

#include <string>

namespace ros2_julia
{

class Publisher
{
public:
  Publisher(
    Node& node,
    const std::string& topic,
    const void* type_support);
  ~Publisher();

  Publisher(const Publisher&) = delete;
  Publisher& operator=(const Publisher&) = delete;

  Publisher(Publisher&&) = delete;
  Publisher& operator=(Publisher&&) = delete;

  void publish(void* msg);

  rcl_publisher_t* rcl_publisher();
  const rcl_publisher_t* rcl_publisher() const;

private:
  Node* node_ = nullptr;
  rcl_publisher_t publisher_;
  bool initialized_ = false;
};

}  // namespace ros2_julia
