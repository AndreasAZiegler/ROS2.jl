#include <ros2_julia/action_client.hpp>
#include <ros2_julia/action_server.hpp>
#include <ros2_julia/context.hpp>
#include <ros2_julia/executor.hpp>
#include <ros2_julia/messages.hpp>
#include <ros2_julia/fibonacci_msgs.hpp>
#include <ros2_julia/node.hpp>
#include <ros2_julia/publisher.hpp>
#include <ros2_julia/request_header.hpp>
#include <ros2_julia/subscription.hpp>

#include <jlcxx/jlcxx.hpp>

#include <rcutils/allocator.h>
#include <rcutils/error_handling.h>
#include <rcutils/shared_library.h>

#include <rosidl_runtime_c/action_type_support_struct.h>

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace ros2_julia
{

static void parse_ros2_interface_name(
  const std::string& name,
  std::string& package,
  std::string& interface_type,
  std::string& interface_name)
{
  // Accept: pkg/Name, pkg/msg/Name, pkg/srv/Name, pkg/action/Name
  // Normalize to 3 parts: pkg, type, name.
  size_t first = name.find('/');
  if (first == std::string::npos) {
    throw std::runtime_error("invalid interface name (expected pkg/...): " + name);
  }
  package = name.substr(0, first);

  std::string rest = name.substr(first + 1);
  size_t second = rest.find('/');
  if (second == std::string::npos) {
    // pkg/Name
    interface_type = "";
    interface_name = rest;
    return;
  }

  interface_type = rest.substr(0, second);
  interface_name = rest.substr(second + 1);
}

static rcutils_shared_library_t* get_or_load_typesupport_library(
  const std::string& package,
  const std::string& typesupport_name)
{
  // Cache per (package, typesupport_name)
  static std::unordered_map<std::string, rcutils_shared_library_t> libs;

  const std::string key = package + "::" + typesupport_name;
  auto it = libs.find(key);
  if (it != libs.end()) {
    return &it->second;
  }

  rcutils_shared_library_t lib = rcutils_get_zero_initialized_shared_library();
  const std::string lib_base = package + "__" + typesupport_name;

  // rcutils does platform-specific prefix/suffix handling.
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  rcutils_ret_t rc = rcutils_load_shared_library(&lib, lib_base.c_str(), allocator);
  if (rc != RCUTILS_RET_OK) {
    // Clear error state before retrying, otherwise later errors get overwritten warnings.
    rcutils_reset_error();

    // Try loading by full filename as a fallback (Linux default).
    const std::string fallback = "lib" + lib_base + ".so";
    rc = rcutils_load_shared_library(&lib, fallback.c_str(), allocator);
  }

  // If we recovered from a previous failure, clear the stale rcutils error message.
  if (rc == RCUTILS_RET_OK) {
    rcutils_reset_error();
  }

  if (rc != RCUTILS_RET_OK) {
    throw std::runtime_error("failed to load typesupport library: " + lib_base);
  }

  auto inserted = libs.emplace(key, lib);
  return &inserted.first->second;
}

static const void* get_typesupport_by_name(const std::string& name)
{
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

static const void* get_action_typesupport_dynamic(const std::string& name)
{
  std::string pkg, kind, action_name;
  parse_ros2_interface_name(name, pkg, kind, action_name);

  // If the input was pkg/ActionName, treat it as an action name.
  if (kind.empty()) {
    kind = "action";
  }

  if (kind != "action") {
    throw std::runtime_error("not an action type: " + name);
  }

  // The symbol name is defined by ROSIDL_TYPESUPPORT_INTERFACE__ACTION_SYMBOL_NAME:
  // rosidl_typesupport_c__get_action_type_support_handle__pkg__action__Name
  const std::string sym =
    std::string("rosidl_typesupport_c") +
    "__get_action_type_support_handle__" +
    pkg +
    "__action__" +
    action_name;

  rcutils_shared_library_t* lib = get_or_load_typesupport_library(pkg, "rosidl_typesupport_c");

  void* sym_ptr = rcutils_get_symbol(lib, sym.c_str());
  if (sym_ptr == nullptr) {
    throw std::runtime_error("action typesupport symbol not found: " + sym);
  }

  using fn_t = const rosidl_action_type_support_t* (*)();
  auto fn = reinterpret_cast<fn_t>(sym_ptr);
  const rosidl_action_type_support_t* ts = fn();
  if (ts == nullptr) {
    throw std::runtime_error("action typesupport returned nullptr for: " + name);
  }

  return static_cast<const void*>(ts);
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

  mod.add_type<FibonacciSendGoalRequest>("FibonacciSendGoalRequest")
    .constructor<>()
    .method("set_order", &FibonacciSendGoalRequest::set_order)
    .method("set_goal_id_bytes", &FibonacciSendGoalRequest::set_goal_id_bytes)
    .method("ros_message", static_cast<void* (FibonacciSendGoalRequest::*)()>(&FibonacciSendGoalRequest::ros_message));

  mod.add_type<FibonacciSendGoalResponse>("FibonacciSendGoalResponse")
    .constructor<>()
    .method("set_accepted", &FibonacciSendGoalResponse::set_accepted)
    .method("ros_message", static_cast<void* (FibonacciSendGoalResponse::*)()>(&FibonacciSendGoalResponse::ros_message));

  mod.add_type<FibonacciGetResultRequest>("FibonacciGetResultRequest")
    .constructor<>()
    .method("set_goal_id_bytes", &FibonacciGetResultRequest::set_goal_id_bytes)
    .method("ros_message", static_cast<void* (FibonacciGetResultRequest::*)()>(&FibonacciGetResultRequest::ros_message));

  mod.add_type<FibonacciGetResultResponse>("FibonacciGetResultResponse")
    .constructor<>()
    .method("set_status", &FibonacciGetResultResponse::set_status)
    .method("set_sequence_fibonacci", &FibonacciGetResultResponse::set_sequence_fibonacci)
    .method("sequence_length", &FibonacciGetResultResponse::sequence_length)
    .method("sequence_at", &FibonacciGetResultResponse::sequence_at)
    .method("ros_message", static_cast<void* (FibonacciGetResultResponse::*)()>(&FibonacciGetResultResponse::ros_message));

  mod.add_type<RequestHeader>("RequestHeader")
    .constructor<>();

  mod.add_type<ActionClient>("ActionClient")
    .constructor<Node&, std::string, const void*>()
    .method("send_goal_request", &ActionClient::send_goal_request)
    .method("send_result_request", &ActionClient::send_result_request)
    .method("send_cancel_request", &ActionClient::send_cancel_request)
    .method("take_goal_response", &ActionClient::take_goal_response)
    .method("take_result_response", &ActionClient::take_result_response)
    .method("take_cancel_response", &ActionClient::take_cancel_response)
    .method("take_feedback", &ActionClient::take_feedback)
    .method("take_status", &ActionClient::take_status);

  mod.add_type<ActionServer>("ActionServer")
    .constructor<Node&, std::string, const void*>()
    .method("take_goal_request", &ActionServer::take_goal_request)
    .method("take_cancel_request", &ActionServer::take_cancel_request)
    .method("take_result_request", &ActionServer::take_result_request)
    .method("send_goal_response", &ActionServer::send_goal_response)
    .method("send_cancel_response", &ActionServer::send_cancel_response)
    .method("send_result_response", &ActionServer::send_result_response)
    .method("accept_new_goal", &ActionServer::accept_new_goal)
    .method("finalize_goal_handle", &ActionServer::finalize_goal_handle)
    .method("publish_feedback", &ActionServer::publish_feedback)
    .method("publish_status", &ActionServer::publish_status)
    .method("notify_goal_done", &ActionServer::notify_goal_done);

  mod.method("typesupport_std_msgs_string", &typesupport_std_msgs_string);

  // Use distinct names to avoid collisions with Julia convenience wrappers.
  mod.method("_cpp_get_typesupport", &get_typesupport_by_name);
  mod.method("_cpp_get_action_typesupport", &get_action_typesupport_dynamic);
}
