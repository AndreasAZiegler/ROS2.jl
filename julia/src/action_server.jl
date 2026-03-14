export ActionServerHandle, create_action_server,
       take_goal_request, take_cancel_request, take_result_request,
       send_goal_response, send_cancel_response, send_result_response,
       accept_new_goal, finalize_goal_handle,
       publish_feedback, publish_status, notify_goal_done

"""
    ActionServerHandle

Owns the creating node to ensure the node/context outlive the action server.
"""
struct ActionServerHandle
    node::NodeHandle
    impl::ActionServer
end

"""
    create_action_server(node, action_typesupport::Ptr{Cvoid}, action_name::AbstractString)

`action_typesupport` is a `rosidl_action_type_support_t*` as `Ptr{Cvoid}`.
"""
function create_action_server(node::NodeHandle, action_typesupport::Ptr{Cvoid}, action_name::AbstractString)
    impl = ActionServer(node.impl, String(action_name), action_typesupport)
    return ActionServerHandle(node, impl)
end

# Take requests

take_goal_request(as::ActionServerHandle, hdr::RequestHeader, goal_req::Ptr{Cvoid}) = ROS2.take_goal_request(as.impl, hdr, goal_req)
take_cancel_request(as::ActionServerHandle, hdr::RequestHeader, cancel_req::Ptr{Cvoid}) = ROS2.take_cancel_request(as.impl, hdr, cancel_req)
take_result_request(as::ActionServerHandle, hdr::RequestHeader, result_req::Ptr{Cvoid}) = ROS2.take_result_request(as.impl, hdr, result_req)

# Send responses

send_goal_response(as::ActionServerHandle, hdr::RequestHeader, goal_resp::Ptr{Cvoid}) = (ROS2.send_goal_response(as.impl, hdr, goal_resp); nothing)
send_cancel_response(as::ActionServerHandle, hdr::RequestHeader, cancel_resp::Ptr{Cvoid}) = (ROS2.send_cancel_response(as.impl, hdr, cancel_resp); nothing)
send_result_response(as::ActionServerHandle, hdr::RequestHeader, result_resp::Ptr{Cvoid}) = (ROS2.send_result_response(as.impl, hdr, result_resp); nothing)

# Goal handle lifecycle

accept_new_goal(as::ActionServerHandle, goal_info_ptr::Ptr{Cvoid}) = ROS2.accept_new_goal(as.impl, goal_info_ptr)
finalize_goal_handle(as::ActionServerHandle, goal_handle::Ptr{Cvoid}) = (ROS2.finalize_goal_handle(as.impl, goal_handle); nothing)

# Feedback/status

publish_feedback(as::ActionServerHandle, feedback_msg::Ptr{Cvoid}) = (ROS2.publish_feedback(as.impl, feedback_msg); nothing)
publish_status(as::ActionServerHandle, status_msg::Ptr{Cvoid}) = (ROS2.publish_status(as.impl, status_msg); nothing)

notify_goal_done(as::ActionServerHandle) = (ROS2.notify_goal_done(as.impl); nothing)
