#include <ros2_julia/node.hpp>
#include <ros2_julia/utils.hpp>

#include <rcl/node_options.h>

namespace ros2_julia
{

Node::Node(Context& ctx, const std::string& name)
{
  node_ = rcl_get_zero_initialized_node();
  const rcl_node_options_t options = rcl_node_get_default_options();
  throw_on_rcl_error(
    rcl_node_init(&node_, name.c_str(), "", ctx.rcl_context(), &options),
    "rcl_node_init");
}

Node::~Node()
{
  (void)rcl_node_fini(&node_);
}

std::string Node::get_name() const
{
  const char* n = rcl_node_get_name(&node_);
  return n != nullptr ? std::string(n) : std::string();
}

rcl_node_t* Node::rcl_node()
{
  return &node_;
}

const rcl_node_t* Node::rcl_node() const
{
  return &node_;
}

}  // namespace ros2_julia

