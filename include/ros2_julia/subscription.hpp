#pragma once

#include <ros2_julia/node.hpp>

#include <rcl/subscription.h>

#include <string>

namespace ros2_julia
{

class Subscription
{
public:
  Subscription(
    Node& node,
    const std::string& topic,
    const void* type_support);
  ~Subscription();

  Subscription(const Subscription&) = delete;
  Subscription& operator=(const Subscription&) = delete;

  Subscription(Subscription&&) = delete;
  Subscription& operator=(Subscription&&) = delete;

  // Returns true if a message was taken successfully, false if no message was available.
  bool take(void* msg);

  rcl_subscription_t* rcl_subscription();
  const rcl_subscription_t* rcl_subscription() const;

private:
  Node* node_ = nullptr;
  rcl_subscription_t subscription_;
  bool initialized_ = false;
};

}  // namespace ros2_julia
