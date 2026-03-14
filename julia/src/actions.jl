export get_action_typesupport

"""
    get_action_typesupport(action_type::AbstractString) -> Ptr{Cvoid}

Return a ROS2 action typesupport handle pointer.

The C++ shim returns an opaque pointer wrapper (`ConstCxxPtr`); we unwrap it to
its raw pointer field (`cpp_object`) for passing back into C++.
"""
function get_action_typesupport(action_type::AbstractString)::Ptr{Cvoid}
    ts = _cpp_get_action_typesupport(String(action_type))
    return ts.cpp_object
end
