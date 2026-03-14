#include <ros2_julia/action_client.hpp>
#include <ros2_julia/utils.hpp>

#include <rcl_action/action_client.h>
// #include <rcl_action/client_options.h>

namespace ros2_julia {

ActionClient::ActionClient(Node &node, const std::string &action_name,
                           const void *action_type_support) {
  node_ = &node;
  client_ = rcl_action_get_zero_initialized_client();

  const rcl_action_client_options_t options =
      rcl_action_client_get_default_options();
  auto ts =
      static_cast<const rosidl_action_type_support_t *>(action_type_support);

  throw_on_rcl_error(rcl_action_client_init(&client_, node.rcl_node(), ts,
                                            action_name.c_str(), &options),
                     "rcl_action_client_init");
  initialized_ = true;
}

ActionClient::~ActionClient() {
  if (initialized_) {
    (void)rcl_action_client_fini(&client_, node_ ? node_->rcl_node() : nullptr);
  }
}

int64_t ActionClient::send_goal_request(void *goal_request) {
  int64_t seq = 0;
  throw_on_rcl_error(rcl_action_send_goal_request(&client_, goal_request, &seq),
                     "rcl_action_send_goal_request");
  return seq;
}

int64_t ActionClient::send_result_request(void *result_request) {
  int64_t seq = 0;
  throw_on_rcl_error(
      rcl_action_send_result_request(&client_, result_request, &seq),
      "rcl_action_send_result_request");
  return seq;
}

int64_t ActionClient::send_cancel_request(void *cancel_request) {
  int64_t seq = 0;
  throw_on_rcl_error(
      rcl_action_send_cancel_request(&client_, cancel_request, &seq),
      "rcl_action_send_cancel_request");
  return seq;
}

bool ActionClient::take_goal_response(RequestHeader &header,
                                      void *goal_response) {
  rcl_ret_t ret = rcl_action_take_goal_response(
      &client_, header.rmw_request_id(), goal_response);
  if (ret == RCL_RET_OK) {
    return true;
  }
  if (ret == RCL_RET_ACTION_CLIENT_TAKE_FAILED) {
    return false;
  }
  throw_on_rcl_error(ret, "rcl_action_take_goal_response");
  return false;
}

bool ActionClient::take_result_response(RequestHeader &header,
                                        void *result_response) {
  rcl_ret_t ret = rcl_action_take_result_response(
      &client_, header.rmw_request_id(), result_response);
  if (ret == RCL_RET_OK) {
    return true;
  }
  if (ret == RCL_RET_ACTION_CLIENT_TAKE_FAILED) {
    return false;
  }
  throw_on_rcl_error(ret, "rcl_action_take_result_response");
  return false;
}

bool ActionClient::take_cancel_response(RequestHeader &header,
                                        void *cancel_response) {
  rcl_ret_t ret = rcl_action_take_cancel_response(
      &client_, header.rmw_request_id(), cancel_response);
  if (ret == RCL_RET_OK) {
    return true;
  }
  if (ret == RCL_RET_ACTION_CLIENT_TAKE_FAILED) {
    return false;
  }
  throw_on_rcl_error(ret, "rcl_action_take_cancel_response");
  return false;
}

bool ActionClient::take_feedback(void *feedback_message) {
  rcl_ret_t ret = rcl_action_take_feedback(&client_, feedback_message);
  if (ret == RCL_RET_OK) {
    return true;
  }
  if (ret == RCL_RET_ACTION_CLIENT_TAKE_FAILED) {
    return false;
  }
  throw_on_rcl_error(ret, "rcl_action_take_feedback");
  return false;
}

bool ActionClient::take_status(void *status_message) {
  rcl_ret_t ret = rcl_action_take_status(&client_, status_message);
  if (ret == RCL_RET_OK) {
    return true;
  }
  if (ret == RCL_RET_ACTION_CLIENT_TAKE_FAILED) {
    return false;
  }
  throw_on_rcl_error(ret, "rcl_action_take_status");
  return false;
}

rcl_action_client_t *ActionClient::rcl_action_client() { return &client_; }

const rcl_action_client_t *ActionClient::rcl_action_client() const {
  return &client_;
}

} // namespace ros2_julia
