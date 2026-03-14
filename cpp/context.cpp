#include <ros2_julia/context.hpp>
#include <ros2_julia/utils.hpp>

#include <rcl/init_options.h>

namespace ros2_julia
{

Context::Context()
{
  context_ = rcl_get_zero_initialized_context();

  rcl_allocator_t allocator = rcl_get_default_allocator();
  rcl_init_options_t options = rcl_get_zero_initialized_init_options();

  throw_on_rcl_error(rcl_init_options_init(&options, allocator), "rcl_init_options_init");

  // No args for now; a higher-level Julia layer can provide argv later if desired.
  throw_on_rcl_error(rcl_init(0, nullptr, &options, &context_), "rcl_init");

  // Options can be finalized after init.
  throw_on_rcl_error(rcl_init_options_fini(&options), "rcl_init_options_fini");
}

Context::~Context()
{
  // Best-effort shutdown/fini; never throw from destructor.
  if (rcl_context_is_valid(&context_)) {
    (void)rcl_shutdown(&context_);
  }
  (void)rcl_context_fini(&context_);
}

rcl_context_t* Context::rcl_context()
{
  return &context_;
}

const rcl_context_t* Context::rcl_context() const
{
  return &context_;
}

}  // namespace ros2_julia

