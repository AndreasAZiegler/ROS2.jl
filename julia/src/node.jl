export NodeHandle, Node, create_node, node_name

"""
    NodeHandle

Owns a `Context` and a wrapped `Node` to ensure the context outlives the node.
"""
struct NodeHandle
    ctx::Context
    impl::Node
end

"""
    Node(name::AbstractString) -> NodeHandle
"""
function Node(name::AbstractString)
    ctx = Context()
    impl = Node(ctx, String(name))  # CxxWrap constructor
    return NodeHandle(ctx, impl)
end

"""
    create_node(ctx::Context, name::AbstractString) -> NodeHandle

Create a node using an existing context.
"""
function create_node(ctx::Context, name::AbstractString)
    impl = Node(ctx, String(name))  # CxxWrap constructor
    return NodeHandle(ctx, impl)
end

node_name(nh::NodeHandle) = ROS2.get_name(nh.impl)
