#pragma once

#include <rcl/types.h>

#include <string>

namespace ros2_julia {

// Throws std::runtime_error on non-OK return codes with the current rcl error
// string appended (if present).
void throw_on_rcl_error(rcl_ret_t ret, const char *what);

} // namespace ros2_julia
