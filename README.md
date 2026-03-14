# ROS2.jl (rcl via CxxWrap)

Thin Julia bindings to the ROS2 `rcl` C API via a small C++ shim built with
`libcxxwrap-julia` / `CxxWrap.jl`.

This repository is a scaffold intended to mirror the architecture of existing
ROS2 client libraries:

- Julia provides the high-level API and callback integration.
- C++ provides RAII wrappers around `rcl_*` structs and exposes them via CxxWrap.

## Status

Core wrappers included:

- `Context` (`rcl_init`, `rcl_shutdown`, `rcl_context_fini`)
- `Node` (`rcl_node_init`, `rcl_node_fini`)
- `Publisher` (`rcl_publisher_init`, `rcl_publish`, `rcl_publisher_fini`)
- `Subscription` (`rcl_subscription_init`, `rcl_take`, `rcl_subscription_fini`)
- `Executor` (minimal `rcl_wait_set`-based wait and readiness tracking)

Message generation and type support are scaffolded and currently include a
single mapping for `std_msgs/msg/String` when available at build time.

## Build (C++)

Prereqs:

- ROS2 C libraries/headers: `rcl`, `rcutils`, `rmw`, `rosidl_runtime_c`, `rosidl_typesupport_c`
- `libcxxwrap-julia` (provides CMake package `JlCxx`)

Configure and build:

```sh
cd ros2_julia
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

This produces `build/libros2_julia.so` (or platform equivalent).

## Use (Julia)

From `ros2_julia/julia`:

```julia
using Pkg
Pkg.activate(".")
Pkg.instantiate()

using ROS2
```

## Notes

- This repo intentionally does not include high-level ROS logic in C++.
- For real message support you will want the generator to emit both Julia
  structs and C++ glue to obtain typesupport handles per message.

