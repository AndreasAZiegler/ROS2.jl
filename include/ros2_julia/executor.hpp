#pragma once

#include <ros2_julia/context.hpp>
#include <ros2_julia/subscription.hpp>

#include <unordered_set>
#include <vector>

namespace ros2_julia
{

class Executor
{
public:
  explicit Executor(Context& ctx);

  void add_subscription(Subscription& sub);
  void spin_once(int timeout_ms);

  // After spin_once(), indicates whether the subscription was marked ready.
  bool is_ready(const Subscription& sub) const;

private:
  Context& ctx_;
  std::vector<Subscription*> subscriptions_;
  std::unordered_set<const Subscription*> ready_;
};

}  // namespace ros2_julia

