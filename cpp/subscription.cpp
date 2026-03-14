#include <ros2_julia/subscription.hpp>
#include <ros2_julia/utils.hpp>

#include <rosidl_runtime_c/message_type_support_struct.h>

namespace ros2_julia {

Subscription::Subscription(Node &node, const std::string &topic,
                           const void *type_support) {
  node_ = &node;
  subscription_ = rcl_get_zero_initialized_subscription();

  const rcl_subscription_options_t options =
      rcl_subscription_get_default_options();
  auto ts = static_cast<const rosidl_message_type_support_t *>(type_support);

  throw_on_rcl_error(rcl_subscription_init(&subscription_, node.rcl_node(), ts,
                                           topic.c_str(), &options),
                     "rcl_subscription_init");
  initialized_ = true;
}

Subscription::~Subscription() {
  if (initialized_) {
    (void)rcl_subscription_fini(&subscription_,
                                node_ ? node_->rcl_node() : nullptr);
  }
}

bool Subscription::take(void *msg) {
  rcl_ret_t ret = rcl_take(&subscription_, msg, nullptr, nullptr);
  if (ret == RCL_RET_OK) {
    return true;
  }
  if (ret == RCL_RET_SUBSCRIPTION_TAKE_FAILED) {
    return false;
  }
  throw_on_rcl_error(ret, "rcl_take");
  return false;
}

rcl_subscription_t *Subscription::rcl_subscription() { return &subscription_; }

const rcl_subscription_t *Subscription::rcl_subscription() const {
  return &subscription_;
}

} // namespace ros2_julia
