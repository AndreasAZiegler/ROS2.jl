#include <ros2_julia/utils.hpp>

#include <rcl/error_handling.h>

#include <stdexcept>

namespace ros2_julia
{

void throw_on_rcl_error(rcl_ret_t ret, const char* what)
{
  if (ret == RCL_RET_OK) {
    return;
  }
  const rcl_error_string_t err = rcl_get_error_string();
  std::string msg = std::string(what) + " failed (rcl_ret_t=" + std::to_string(static_cast<int>(ret)) + ")";
  if (err.str != nullptr && err.str[0] != '\0') {
    msg += ": ";
    msg += err.str;
  }
  rcl_reset_error();
  throw std::runtime_error(msg);
}

}  // namespace ros2_julia

