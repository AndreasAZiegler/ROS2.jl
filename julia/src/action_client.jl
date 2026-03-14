export ActionClientHandle, create_action_client,
       send_goal_request, send_result_request, send_cancel_request,
       take_goal_response, take_result_response, take_cancel_response,
       take_feedback, take_status

"""
    ActionClientHandle

Owns the creating node to ensure the node/context outlive the action client.
"""
struct ActionClientHandle
    node::NodeHandle
    impl::ActionClient
end

"""
    create_action_client(node, action_typesupport::Ptr{Cvoid}, action_name::AbstractString)

`action_typesupport` is a `rosidl_action_type_support_t*` as `Ptr{Cvoid}`.
"""
function create_action_client(node::NodeHandle, action_typesupport::Ptr{Cvoid}, action_name::AbstractString)
    impl = ActionClient(node.impl, String(action_name), action_typesupport)
    return ActionClientHandle(node, impl)
end

send_goal_request(ac::ActionClientHandle, goal_req::Ptr{Cvoid}) = ROS2.send_goal_request(ac.impl, goal_req)
send_result_request(ac::ActionClientHandle, result_req::Ptr{Cvoid}) = ROS2.send_result_request(ac.impl, result_req)
send_cancel_request(ac::ActionClientHandle, cancel_req::Ptr{Cvoid}) = ROS2.send_cancel_request(ac.impl, cancel_req)

take_goal_response(ac::ActionClientHandle, hdr::RequestHeader, goal_resp::Ptr{Cvoid}) = ROS2.take_goal_response(ac.impl, hdr, goal_resp)
take_result_response(ac::ActionClientHandle, hdr::RequestHeader, result_resp::Ptr{Cvoid}) = ROS2.take_result_response(ac.impl, hdr, result_resp)
take_cancel_response(ac::ActionClientHandle, hdr::RequestHeader, cancel_resp::Ptr{Cvoid}) = ROS2.take_cancel_response(ac.impl, hdr, cancel_resp)

take_feedback(ac::ActionClientHandle, feedback_msg::Ptr{Cvoid}) = ROS2.take_feedback(ac.impl, feedback_msg)
take_status(ac::ActionClientHandle, status_msg::Ptr{Cvoid}) = ROS2.take_status(ac.impl, status_msg)
