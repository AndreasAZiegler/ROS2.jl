#pragma once

#include <ros2_julia/node.hpp>
#include <ros2_julia/request_header.hpp>

#include <rcl_action/action_client.h>
#include <rosidl_runtime_c/action_type_support_struct.h>

#include <cstdint>
#include <string>

namespace ros2_julia {

class ActionClient {
public:
  ActionClient(Node &node, const std::string &action_name,
               const void *action_type_support);
  ~ActionClient();

  ActionClient(const ActionClient &) = delete;
  ActionClient &operator=(const ActionClient &) = delete;

  ActionClient(ActionClient &&) = delete;
  ActionClient &operator=(ActionClient &&) = delete;

  // Send requests. Return the rcl sequence number.
  int64_t send_goal_request(void *goal_request);
  int64_t send_result_request(void *result_request);
  int64_t send_cancel_request(void *cancel_request);

  // Take responses / messages. Return true if a message was taken.
  bool take_goal_response(RequestHeader &header, void *goal_response);
  bool take_result_response(RequestHeader &header, void *result_response);
  bool take_cancel_response(RequestHeader &header, void *cancel_response);

  bool take_feedback(void *feedback_message);
  bool take_status(void *status_message);

  rcl_action_client_t *rcl_action_client();
  const rcl_action_client_t *rcl_action_client() const;

private:
  Node *node_ = nullptr;
  rcl_action_client_t client_;
  bool initialized_ = false;
};

} // namespace ros2_julia
