#include <ros2_julia/executor.hpp>
#include <ros2_julia/utils.hpp>

#include <rcl/types.h>
#include <rcl/wait.h>
// #include <rcl/wait_set.h>

namespace ros2_julia {

Executor::Executor(Context &ctx) : ctx_(ctx) {}

void Executor::add_subscription(Subscription &sub) {
  subscriptions_.push_back(&sub);
}

void Executor::spin_once(int timeout_ms) {
  ready_.clear();

  const size_t nsubs = subscriptions_.size();
  if (nsubs == 0) {
    // rcl_wait on an empty wait set returns RCL_RET_WAIT_SET_EMPTY; treat as a
    // no-op.
    (void)timeout_ms;
    return;
  }

  rcl_allocator_t allocator = rcl_get_default_allocator();
  rcl_wait_set_t wait_set = rcl_get_zero_initialized_wait_set();

  throw_on_rcl_error(rcl_wait_set_init(&wait_set, nsubs, 0, 0, 0, 0, 0,
                                       ctx_.rcl_context(), allocator),
                     "rcl_wait_set_init");
  throw_on_rcl_error(rcl_wait_set_clear(&wait_set), "rcl_wait_set_clear");

  // rcl_wait_set_add_subscription returns the index slot used in
  // wait_set.subscriptions.
  std::vector<size_t> indices;
  indices.reserve(nsubs);
  for (size_t i = 0; i < nsubs; ++i) {
    size_t index = 0;
    throw_on_rcl_error(
        rcl_wait_set_add_subscription(
            &wait_set, subscriptions_[i]->rcl_subscription(), &index),
        "rcl_wait_set_add_subscription");
    indices.push_back(index);
  }

  const int64_t timeout_ns =
      timeout_ms < 0 ? -1 : static_cast<int64_t>(timeout_ms) * 1000000LL;
  rcl_ret_t wait_ret = rcl_wait(&wait_set, timeout_ns);

  // TIMEOUT is not an error; it just means nothing became ready before the
  // deadline.
  if (wait_ret == RCL_RET_TIMEOUT) {
    (void)rcl_wait_set_fini(&wait_set);
    return;
  }
  throw_on_rcl_error(wait_ret, "rcl_wait");

  for (size_t i = 0; i < nsubs; ++i) {
    const size_t idx = indices[i];
    if (idx < wait_set.size_of_subscriptions &&
        wait_set.subscriptions[idx] != nullptr) {
      ready_.insert(subscriptions_[i]);
    }
  }

  (void)rcl_wait_set_fini(&wait_set);
}

bool Executor::is_ready(const Subscription &sub) const {
  return ready_.find(&sub) != ready_.end();
}

} // namespace ros2_julia
