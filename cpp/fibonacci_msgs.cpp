#include <ros2_julia/fibonacci_msgs.hpp>

#include <stdexcept>

#if __has_include(<example_interfaces/action/fibonacci.h>)
  #define ROS2_JULIA_HAS_EXAMPLE_FIBONACCI 1
  #include <example_interfaces/action/fibonacci.h>
  #include <rosidl_runtime_c/primitives_sequence_functions.h>
#else
  #define ROS2_JULIA_HAS_EXAMPLE_FIBONACCI 0
#endif

namespace ros2_julia
{

#if ROS2_JULIA_HAS_EXAMPLE_FIBONACCI

struct FibonacciSendGoalRequest::Impl
{
  example_interfaces__action__Fibonacci_SendGoal_Request msg;
  bool initialized = false;
};

FibonacciSendGoalRequest::FibonacciSendGoalRequest()
{
  impl_ = new Impl();
  if (!example_interfaces__action__Fibonacci_SendGoal_Request__init(&impl_->msg)) {
    delete impl_;
    impl_ = nullptr;
    throw std::runtime_error("Fibonacci_SendGoal_Request__init failed");
  }
  impl_->initialized = true;
}

FibonacciSendGoalRequest::~FibonacciSendGoalRequest()
{
  if (!impl_) {
    return;
  }
  if (impl_->initialized) {
    example_interfaces__action__Fibonacci_SendGoal_Request__fini(&impl_->msg);
  }
  delete impl_;
  impl_ = nullptr;
}

void FibonacciSendGoalRequest::set_order(int32_t order)
{
  impl_->msg.goal.order = order;
}

void FibonacciSendGoalRequest::set_goal_id_bytes(const uint8_t* data16)
{
  // unique_identifier_msgs/msg/UUID has uint8[16] uuid
  for (size_t i = 0; i < 16; ++i) {
    impl_->msg.goal_id.uuid[i] = data16[i];
  }
}

void* FibonacciSendGoalRequest::ros_message()
{
  return static_cast<void*>(&impl_->msg);
}

const void* FibonacciSendGoalRequest::ros_message() const
{
  return static_cast<const void*>(&impl_->msg);
}

struct FibonacciSendGoalResponse::Impl
{
  example_interfaces__action__Fibonacci_SendGoal_Response msg;
  bool initialized = false;
};

FibonacciSendGoalResponse::FibonacciSendGoalResponse()
{
  impl_ = new Impl();
  if (!example_interfaces__action__Fibonacci_SendGoal_Response__init(&impl_->msg)) {
    delete impl_;
    impl_ = nullptr;
    throw std::runtime_error("Fibonacci_SendGoal_Response__init failed");
  }
  impl_->initialized = true;
}

FibonacciSendGoalResponse::~FibonacciSendGoalResponse()
{
  if (!impl_) {
    return;
  }
  if (impl_->initialized) {
    example_interfaces__action__Fibonacci_SendGoal_Response__fini(&impl_->msg);
  }
  delete impl_;
  impl_ = nullptr;
}

void FibonacciSendGoalResponse::set_accepted(bool accepted)
{
  impl_->msg.accepted = accepted;
  // stamp left as zero; a full implementation would populate from a clock.
}

void* FibonacciSendGoalResponse::ros_message()
{
  return static_cast<void*>(&impl_->msg);
}

const void* FibonacciSendGoalResponse::ros_message() const
{
  return static_cast<const void*>(&impl_->msg);
}

struct FibonacciGetResultRequest::Impl
{
  example_interfaces__action__Fibonacci_GetResult_Request msg;
  bool initialized = false;
};

FibonacciGetResultRequest::FibonacciGetResultRequest()
{
  impl_ = new Impl();
  if (!example_interfaces__action__Fibonacci_GetResult_Request__init(&impl_->msg)) {
    delete impl_;
    impl_ = nullptr;
    throw std::runtime_error("Fibonacci_GetResult_Request__init failed");
  }
  impl_->initialized = true;
}

FibonacciGetResultRequest::~FibonacciGetResultRequest()
{
  if (!impl_) {
    return;
  }
  if (impl_->initialized) {
    example_interfaces__action__Fibonacci_GetResult_Request__fini(&impl_->msg);
  }
  delete impl_;
  impl_ = nullptr;
}

void FibonacciGetResultRequest::set_goal_id_bytes(const uint8_t* data16)
{
  for (size_t i = 0; i < 16; ++i) {
    impl_->msg.goal_id.uuid[i] = data16[i];
  }
}

void* FibonacciGetResultRequest::ros_message()
{
  return static_cast<void*>(&impl_->msg);
}

const void* FibonacciGetResultRequest::ros_message() const
{
  return static_cast<const void*>(&impl_->msg);
}

struct FibonacciGetResultResponse::Impl
{
  example_interfaces__action__Fibonacci_GetResult_Response msg;
  bool initialized = false;
};

FibonacciGetResultResponse::FibonacciGetResultResponse()
{
  impl_ = new Impl();
  if (!example_interfaces__action__Fibonacci_GetResult_Response__init(&impl_->msg)) {
    delete impl_;
    impl_ = nullptr;
    throw std::runtime_error("Fibonacci_GetResult_Response__init failed");
  }
  impl_->initialized = true;
}

FibonacciGetResultResponse::~FibonacciGetResultResponse()
{
  if (!impl_) {
    return;
  }
  if (impl_->initialized) {
    example_interfaces__action__Fibonacci_GetResult_Response__fini(&impl_->msg);
  }
  delete impl_;
  impl_ = nullptr;
}

void FibonacciGetResultResponse::set_status(int8_t status)
{
  impl_->msg.status = status;
}

void FibonacciGetResultResponse::set_sequence_fibonacci(int32_t order)
{
  // Result has: example_interfaces/action/Fibonacci_Result { sequence: int32[] }
  // We fill a simple fibonacci sequence of length order+1.
  if (order < 0) {
    order = 0;
  }
  const size_t n = static_cast<size_t>(order) + 1;

  // Ensure capacity.
  rosidl_runtime_c__int32__Sequence__fini(&impl_->msg.result.sequence);
  if (!rosidl_runtime_c__int32__Sequence__init(&impl_->msg.result.sequence, n)) {
    throw std::runtime_error("int32 sequence init failed");
  }

  if (n >= 1) {
    impl_->msg.result.sequence.data[0] = 0;
  }
  if (n >= 2) {
    impl_->msg.result.sequence.data[1] = 1;
  }
  for (size_t i = 2; i < n; ++i) {
    impl_->msg.result.sequence.data[i] = impl_->msg.result.sequence.data[i - 1] + impl_->msg.result.sequence.data[i - 2];
  }
}

size_t FibonacciGetResultResponse::sequence_length() const
{
  return impl_->msg.result.sequence.size;
}

int32_t FibonacciGetResultResponse::sequence_at(size_t idx) const
{
  if (idx >= impl_->msg.result.sequence.size) {
    throw std::out_of_range("sequence index out of range");
  }
  return impl_->msg.result.sequence.data[idx];
}

void* FibonacciGetResultResponse::ros_message()
{
  return static_cast<void*>(&impl_->msg);
}

const void* FibonacciGetResultResponse::ros_message() const
{
  return static_cast<const void*>(&impl_->msg);
}

#else

// Build-time fallback: compile, but throw if used.

struct FibonacciSendGoalRequest::Impl {};
struct FibonacciSendGoalResponse::Impl {};
struct FibonacciGetResultRequest::Impl {};
struct FibonacciGetResultResponse::Impl {};

static void throw_unavailable()
{
  throw std::runtime_error("example_interfaces/action/fibonacci.h not found at build time");
}

FibonacciSendGoalRequest::FibonacciSendGoalRequest() { throw_unavailable(); }
FibonacciSendGoalRequest::~FibonacciSendGoalRequest() {}
void FibonacciSendGoalRequest::set_order(int32_t) { throw_unavailable(); }
void FibonacciSendGoalRequest::set_goal_id_bytes(const uint8_t*) { throw_unavailable(); }
void* FibonacciSendGoalRequest::ros_message() { throw_unavailable(); return nullptr; }
const void* FibonacciSendGoalRequest::ros_message() const { throw_unavailable(); return nullptr; }

FibonacciSendGoalResponse::FibonacciSendGoalResponse() { throw_unavailable(); }
FibonacciSendGoalResponse::~FibonacciSendGoalResponse() {}
void FibonacciSendGoalResponse::set_accepted(bool) { throw_unavailable(); }
void* FibonacciSendGoalResponse::ros_message() { throw_unavailable(); return nullptr; }
const void* FibonacciSendGoalResponse::ros_message() const { throw_unavailable(); return nullptr; }

FibonacciGetResultRequest::FibonacciGetResultRequest() { throw_unavailable(); }
FibonacciGetResultRequest::~FibonacciGetResultRequest() {}
void FibonacciGetResultRequest::set_goal_id_bytes(const uint8_t*) { throw_unavailable(); }
void* FibonacciGetResultRequest::ros_message() { throw_unavailable(); return nullptr; }
const void* FibonacciGetResultRequest::ros_message() const { throw_unavailable(); return nullptr; }

FibonacciGetResultResponse::FibonacciGetResultResponse() { throw_unavailable(); }
FibonacciGetResultResponse::~FibonacciGetResultResponse() {}
void FibonacciGetResultResponse::set_status(int8_t) { throw_unavailable(); }
void FibonacciGetResultResponse::set_sequence_fibonacci(int32_t) { throw_unavailable(); }
size_t FibonacciGetResultResponse::sequence_length() const { throw_unavailable(); return 0; }
int32_t FibonacciGetResultResponse::sequence_at(size_t) const { throw_unavailable(); return 0; }
void* FibonacciGetResultResponse::ros_message() { throw_unavailable(); return nullptr; }
const void* FibonacciGetResultResponse::ros_message() const { throw_unavailable(); return nullptr; }

#endif

}  // namespace ros2_julia
