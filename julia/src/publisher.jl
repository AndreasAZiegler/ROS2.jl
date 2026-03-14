export PublisherHandle, create_publisher, publish

"""
    PublisherHandle{T}

Owns the creating node to ensure the node/context outlive the publisher.
"""
struct PublisherHandle{T}
    node::NodeHandle
    impl::Publisher
end

function create_publisher(node::NodeHandle, ::Type{StringMsg}, topic::AbstractString)
    ts = get_typesupport(StringMsg)
    impl = Publisher(node.impl, String(topic), ts)
    return PublisherHandle{StringMsg}(node, impl)
end

function publish(pub::PublisherHandle{StringMsg}, msg::StringMsg)
    rosmsg = to_ros_message(msg)
    ROS2.publish(pub.impl, ROS2.ros_message(rosmsg))
    return nothing
end
