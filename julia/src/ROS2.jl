module ROS2

using CxxWrap
using Libdl

_libext() = (startswith(Libdl.dlext, ".") ? Libdl.dlext : "." * Libdl.dlext)

function _maybe_load_ros2_deps()
    # Best-effort: preload ROS2 shared libs so dlopen(libros2_julia.so) can resolve
    # dependencies even if the user didn't source a ROS2 setup script.
    ext = _libext()

    prefixes = String[]
    if haskey(ENV, "ROS2_PREFIX")
        push!(prefixes, ENV["ROS2_PREFIX"])
    end

    if isdir("/opt/ros")
        for p in sort(readdir("/opt/ros"; join=true))
            isdir(p) && push!(prefixes, p)
        end
    end

    for prefix in prefixes
        libdir = joinpath(prefix, "lib")
        isdir(libdir) || continue

        # If we can see librcl, assume it's a usable ROS2 prefix.
        isfile(joinpath(libdir, "librcl" * ext)) || continue

        for libbase in (
            "librcutils",
            "librmw",
            "librosidl_runtime_c",
            "librosidl_typesupport_c",
            "librcl",
        )
            libpath = joinpath(libdir, libbase * ext)
            if isfile(libpath)
                try
                    Libdl.dlopen(libpath)
                catch
                    # Ignore; if this prefix is broken we'll fail later when loading the wrapper.
                end
            end
        end
        return
    end
end

function _find_libros2_julia()
    libfile = "libros2_julia" * _libext()

    candidates = String[]
    if haskey(ENV, "ROS2_JULIA_LIBDIR")
        push!(candidates, joinpath(ENV["ROS2_JULIA_LIBDIR"], libfile))
    end

    # Common in-repo build locations.
    push!(candidates, normpath(joinpath(@__DIR__, "..", "..", "build", libfile)))
    push!(candidates, normpath(joinpath(@__DIR__, "..", "..", "..", "build", libfile)))

    for c in candidates
        if isfile(c)
            return c
        end
    end

    # Fall back to the dynamic loader search path.
    return "libros2_julia"
end

_maybe_load_ros2_deps()

# Load the CxxWrap module from the C++ shim.
@wrapmodule(() -> _find_libros2_julia())

function __init__()
    @initcxx
end

include("context.jl")
include("node.jl")
include("messages.jl")
include("actions.jl")
include("publisher.jl")
include("subscription.jl")
include("executor.jl")
include("action_client.jl")
include("action_server.jl")

end # module ROS2
