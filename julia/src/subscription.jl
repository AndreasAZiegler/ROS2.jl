export SubscriptionHandle, create_subscription, take

"""
    SubscriptionHandle{T}

Owns the creating node to ensure the node/context outlive the subscription.
"""
struct SubscriptionHandle{T}
    node::NodeHandle
    impl::Subscription
end

function create_subscription(node::NodeHandle, ::Type{StringMsg}, topic::AbstractString)
    ts = get_typesupport(StringMsg)
    impl = Subscription(node.impl, String(topic), ts)
    return SubscriptionHandle{StringMsg}(node, impl)
end

"""
    take(sub::SubscriptionHandle{StringMsg}) -> Union{Nothing,StringMsg}
"""
function take(sub::SubscriptionHandle{StringMsg})
    rosmsg = StdMsgsString()
    ok = ROS2.take(sub.impl, ROS2.ros_message(rosmsg))
    return ok ? StringMsg(ROS2.get_data(rosmsg)) : nothing
end
