#include <ros2_julia/context.hpp>
#include <ros2_julia/executor.hpp>
#include <ros2_julia/messages.hpp>
#include <ros2_julia/node.hpp>
#include <ros2_julia/publisher.hpp>
#include <ros2_julia/subscription.hpp>

#include <jlcxx/jlcxx.hpp>

#include <stdexcept>
#include <string>

namespace ros2_julia
{

static const void* get_typesupport_by_name(const std::string& name)
{
  // Minimal proof-of-concept mapping. The generator should extend this table.
  if (name == "std_msgs/String" || name == "std_msgs/msg/String") {
    const void* ts = typesupport_std_msgs_string();
    if (ts == nullptr) {
      throw std::runtime_error(
        "typesupport for std_msgs/msg/String is unavailable (std_msgs not found at build time)");
    }
    return ts;
  }
  throw std::runtime_error("unknown message type: " + name);
}

}  // namespace ros2_julia

JLCXX_MODULE define_julia_module(jlcxx::Module& mod)
{
  using namespace ros2_julia;

  mod.add_type<Context>("Context")
    .constructor<>();

  mod.add_type<Node>("Node")
    .constructor<Context&, std::string>()
    .method("get_name", &Node::get_name);

  // typesupport is passed as Ptr{Cvoid} from Julia.
  mod.add_type<Publisher>("Publisher")
    .constructor<Node&, std::string, const void*>()
    .method("publish", &Publisher::publish);

  mod.add_type<Subscription>("Subscription")
    .constructor<Node&, std::string, const void*>()
    .method("take", &Subscription::take);

  mod.add_type<Executor>("Executor")
    .constructor<Context&>()
    .method("add_subscription", &Executor::add_subscription)
    .method("spin_once", &Executor::spin_once)
    .method("is_ready", &Executor::is_ready);

  mod.add_type<StdMsgsString>("StdMsgsString")
    .constructor<>()
    .constructor<std::string>()
    .method("set_data", &StdMsgsString::set_data)
    .method("get_data", &StdMsgsString::get_data)
    .method("ros_message", static_cast<void* (StdMsgsString::*)()>(&StdMsgsString::ros_message));

  mod.method("typesupport_std_msgs_string", &typesupport_std_msgs_string);

  // Use a distinct name to avoid collisions with Julia convenience wrappers.
  mod.method("_cpp_get_typesupport", &get_typesupport_by_name);
}
