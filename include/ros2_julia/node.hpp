#pragma once

#include <ros2_julia/context.hpp>

#include <rcl/node.h>

#include <string>

namespace ros2_julia
{

class Node
{
public:
  Node(Context& ctx, const std::string& name);
  ~Node();

  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;

  Node(Node&&) = delete;
  Node& operator=(Node&&) = delete;

  std::string get_name() const;

  rcl_node_t* rcl_node();
  const rcl_node_t* rcl_node() const;

private:
  rcl_node_t node_;
};

}  // namespace ros2_julia

