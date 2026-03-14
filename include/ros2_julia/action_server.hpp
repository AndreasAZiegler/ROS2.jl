#pragma once

#include <ros2_julia/node.hpp>
#include <ros2_julia/request_header.hpp>

#include <rcl_action/action_server.h>
#include <rcl_action/types.h>

#include <rcl/time.h>

#include <string>

namespace ros2_julia {

class ActionServer {
public:
  ActionServer(Node &node, const std::string &action_name,
               const void *action_type_support);
  ~ActionServer();

  ActionServer(const ActionServer &) = delete;
  ActionServer &operator=(const ActionServer &) = delete;

  ActionServer(ActionServer &&) = delete;
  ActionServer &operator=(ActionServer &&) = delete;

  // Take incoming requests.
  bool take_goal_request(RequestHeader &header, void *goal_request);
  bool take_cancel_request(RequestHeader &header, void *cancel_request);
  bool take_result_request(RequestHeader &header, void *result_request);

  // Send responses.
  void send_goal_response(RequestHeader &header, void *goal_response);
  void send_cancel_response(RequestHeader &header, void *cancel_response);
  void send_result_response(RequestHeader &header, void *result_response);

  // Goal handle lifecycle.
  // The caller (Julia) supplies goal info extracted from the goal request.
  void *accept_new_goal(const void *goal_info);
  void finalize_goal_handle(void *goal_handle);

  // Publish feedback and status.
  // NOTE (Humble API): feedback/status are full ROS messages. For feedback this
  // typically includes goal_id + feedback payload. For status this is typically
  // action_msgs/msg/GoalStatusArray.
  void publish_feedback(void *feedback_message);
  void publish_status(void *status_message);

  // Notify that goal states changed; triggers status publication in executors.
  void notify_goal_done();

  rcl_action_server_t *rcl_action_server();
  const rcl_action_server_t *rcl_action_server() const;

private:
  Node *node_ = nullptr;
  rcl_action_server_t server_;

  rcl_clock_t clock_;
  bool clock_initialized_ = false;

  bool initialized_ = false;
};

} // namespace ros2_julia
