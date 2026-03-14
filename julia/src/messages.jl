export StringMsg, get_typesupport, to_ros_message

"""
    StringMsg

High-level Julia representation of `std_msgs/msg/String`.
"""
struct StringMsg
    data::String
end

"""
    get_typesupport(type_name::AbstractString) -> Ptr{Cvoid}

Return a ROS2 message typesupport handle pointer for a known message type.

The C++ shim returns an opaque pointer wrapper (`ConstCxxPtr`); we unwrap it to
its raw pointer field (`cpp_object`).
"""
function get_typesupport(type_name::AbstractString)::Ptr{Cvoid}
    ts = _cpp_get_typesupport(String(type_name))
    return ts.cpp_object
end

get_typesupport(::Type{StringMsg}) = get_typesupport("std_msgs/String")

"""
    to_ros_message(msg::StringMsg) -> StdMsgsString

Convert a high-level Julia message to the wrapped ROS2 C message.
"""
to_ros_message(msg::StringMsg) = StdMsgsString(msg.data)
