# High-level helpers around the wrapped C++ Context.

export init, shutdown

"""
    init() -> Context

Create a new ROS2 context (calls `rcl_init` in the C++ layer).
"""
init() = Context()

"""
    shutdown(ctx::Context)

Currently a no-op (shutdown happens in the C++ destructor). Provided for API symmetry.
"""
shutdown(::Context) = nothing

