#pragma once

#include <rcl/context.h>
#include <rcl/init.h>

namespace ros2_julia
{

class Context
{
public:
  Context();
  ~Context();

  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;

  Context(Context&&) = delete;
  Context& operator=(Context&&) = delete;

  rcl_context_t* rcl_context();
  const rcl_context_t* rcl_context() const;

private:
  rcl_context_t context_;
};

}  // namespace ros2_julia

