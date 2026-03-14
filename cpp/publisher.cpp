#include <ros2_julia/publisher.hpp>
#include <ros2_julia/utils.hpp>

#include <rosidl_runtime_c/message_type_support_struct.h>

namespace ros2_julia {

Publisher::Publisher(Node &node, const std::string &topic,
                     const void *type_support) {
  node_ = &node;
  publisher_ = rcl_get_zero_initialized_publisher();

  const rcl_publisher_options_t options = rcl_publisher_get_default_options();
  auto ts = static_cast<const rosidl_message_type_support_t *>(type_support);

  throw_on_rcl_error(rcl_publisher_init(&publisher_, node.rcl_node(), ts,
                                        topic.c_str(), &options),
                     "rcl_publisher_init");
  initialized_ = true;
}

Publisher::~Publisher() {
  if (initialized_) {
    (void)rcl_publisher_fini(&publisher_, node_ ? node_->rcl_node() : nullptr);
  }
}

void Publisher::publish(void *msg) {
  throw_on_rcl_error(rcl_publish(&publisher_, msg, nullptr), "rcl_publish");
}

rcl_publisher_t *Publisher::rcl_publisher() { return &publisher_; }

const rcl_publisher_t *Publisher::rcl_publisher() const { return &publisher_; }

} // namespace ros2_julia
