#include <ros2_julia/action_server.hpp>
#include <ros2_julia/utils.hpp>

#include <rcl_action/action_server.h>
#include <rcl_action/goal_handle.h>
// #include <rcl_action/server_options.h>

#include <rcl/time.h>

#include <stdexcept>

namespace ros2_julia {

ActionServer::ActionServer(Node &node, const std::string &action_name,
                           const void *action_type_support) {
  node_ = &node;
  server_ = rcl_action_get_zero_initialized_server();

  // Humble API requires an explicit clock.
  clock_ = rcl_clock_t{};
  rcl_allocator_t allocator = rcl_get_default_allocator();
  throw_on_rcl_error(rcl_clock_init(RCL_SYSTEM_TIME, &clock_, &allocator),
                     "rcl_clock_init");
  clock_initialized_ = true;

  const rcl_action_server_options_t options =
      rcl_action_server_get_default_options();
  auto ts =
      static_cast<const rosidl_action_type_support_t *>(action_type_support);

  throw_on_rcl_error(rcl_action_server_init(&server_, node.rcl_node(), &clock_,
                                            ts, action_name.c_str(), &options),
                     "rcl_action_server_init");
  initialized_ = true;
}

ActionServer::~ActionServer() {
  // Best-effort; never throw from destructor.
  if (initialized_) {
    (void)rcl_action_server_fini(&server_, node_ ? node_->rcl_node() : nullptr);
  }
  if (clock_initialized_) {
    (void)rcl_clock_fini(&clock_);
  }
}

bool ActionServer::take_goal_request(RequestHeader &header,
                                     void *goal_request) {
  rcl_ret_t ret = rcl_action_take_goal_request(
      &server_, header.rmw_request_id(), goal_request);
  if (ret == RCL_RET_OK) {
    return true;
  }
  if (ret == RCL_RET_ACTION_SERVER_TAKE_FAILED) {
    return false;
  }
  throw_on_rcl_error(ret, "rcl_action_take_goal_request");
  return false;
}

bool ActionServer::take_cancel_request(RequestHeader &header,
                                       void *cancel_request) {
  rcl_ret_t ret = rcl_action_take_cancel_request(
      &server_, header.rmw_request_id(), cancel_request);
  if (ret == RCL_RET_OK) {
    return true;
  }
  if (ret == RCL_RET_ACTION_SERVER_TAKE_FAILED) {
    return false;
  }
  throw_on_rcl_error(ret, "rcl_action_take_cancel_request");
  return false;
}

bool ActionServer::take_result_request(RequestHeader &header,
                                       void *result_request) {
  rcl_ret_t ret = rcl_action_take_result_request(
      &server_, header.rmw_request_id(), result_request);
  if (ret == RCL_RET_OK) {
    return true;
  }
  if (ret == RCL_RET_ACTION_SERVER_TAKE_FAILED) {
    return false;
  }
  throw_on_rcl_error(ret, "rcl_action_take_result_request");
  return false;
}

void ActionServer::send_goal_response(RequestHeader &header,
                                      void *goal_response) {
  throw_on_rcl_error(rcl_action_send_goal_response(
                         &server_, header.rmw_request_id(), goal_response),
                     "rcl_action_send_goal_response");
}

void ActionServer::send_cancel_response(RequestHeader &header,
                                        void *cancel_response) {
  throw_on_rcl_error(rcl_action_send_cancel_response(
                         &server_, header.rmw_request_id(), cancel_response),
                     "rcl_action_send_cancel_response");
}

void ActionServer::send_result_response(RequestHeader &header,
                                        void *result_response) {
  throw_on_rcl_error(rcl_action_send_result_response(
                         &server_, header.rmw_request_id(), result_response),
                     "rcl_action_send_result_response");
}

void *ActionServer::accept_new_goal(const void *goal_info) {
  auto *gi = static_cast<const rcl_action_goal_info_t *>(goal_info);
  rcl_action_goal_handle_t *gh = rcl_action_accept_new_goal(&server_, gi);
  if (gh == nullptr) {
    throw std::runtime_error("rcl_action_accept_new_goal returned nullptr");
  }
  return static_cast<void *>(gh);
}

void ActionServer::finalize_goal_handle(void *goal_handle) {
  auto *gh = static_cast<rcl_action_goal_handle_t *>(goal_handle);
  if (gh == nullptr) {
    return;
  }
  (void)rcl_action_goal_handle_fini(gh);
}

void ActionServer::publish_feedback(void *feedback_message) {
  // Humble API: feedback is a ROS message (typically contains goal_id +
  // feedback payload).
  throw_on_rcl_error(rcl_action_publish_feedback(&server_, feedback_message),
                     "rcl_action_publish_feedback");
}

void ActionServer::publish_status(void *status_message) {
  // Humble API: status message is action_msgs/msg/GoalStatusArray.
  throw_on_rcl_error(rcl_action_publish_status(&server_, status_message),
                     "rcl_action_publish_status");
}

void ActionServer::notify_goal_done() {
  throw_on_rcl_error(rcl_action_notify_goal_done(&server_),
                     "rcl_action_notify_goal_done");
}

rcl_action_server_t *ActionServer::rcl_action_server() { return &server_; }

const rcl_action_server_t *ActionServer::rcl_action_server() const {
  return &server_;
}

} // namespace ros2_julia
