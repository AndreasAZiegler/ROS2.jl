#pragma once

#include <cstdint>
#include <string>

namespace ros2_julia
{

// Proof-of-concept action message wrappers for example_interfaces/action/Fibonacci.
//
// These wrappers exist only to support an end-to-end action client/server test.
// Long term, these should be generated.

class FibonacciSendGoalRequest
{
public:
  FibonacciSendGoalRequest();
  ~FibonacciSendGoalRequest();

  FibonacciSendGoalRequest(const FibonacciSendGoalRequest&) = delete;
  FibonacciSendGoalRequest& operator=(const FibonacciSendGoalRequest&) = delete;

  FibonacciSendGoalRequest(FibonacciSendGoalRequest&&) = delete;
  FibonacciSendGoalRequest& operator=(FibonacciSendGoalRequest&&) = delete;

  void set_order(int32_t order);
  void set_goal_id_bytes(const uint8_t* data16);

  void* ros_message();
  const void* ros_message() const;

private:
  struct Impl;
  Impl* impl_ = nullptr;
};

class FibonacciSendGoalResponse
{
public:
  FibonacciSendGoalResponse();
  ~FibonacciSendGoalResponse();

  FibonacciSendGoalResponse(const FibonacciSendGoalResponse&) = delete;
  FibonacciSendGoalResponse& operator=(const FibonacciSendGoalResponse&) = delete;

  FibonacciSendGoalResponse(FibonacciSendGoalResponse&&) = delete;
  FibonacciSendGoalResponse& operator=(FibonacciSendGoalResponse&&) = delete;

  void set_accepted(bool accepted);

  void* ros_message();
  const void* ros_message() const;

private:
  struct Impl;
  Impl* impl_ = nullptr;
};

class FibonacciGetResultRequest
{
public:
  FibonacciGetResultRequest();
  ~FibonacciGetResultRequest();

  FibonacciGetResultRequest(const FibonacciGetResultRequest&) = delete;
  FibonacciGetResultRequest& operator=(const FibonacciGetResultRequest&) = delete;

  FibonacciGetResultRequest(FibonacciGetResultRequest&&) = delete;
  FibonacciGetResultRequest& operator=(FibonacciGetResultRequest&&) = delete;

  void set_goal_id_bytes(const uint8_t* data16);

  void* ros_message();
  const void* ros_message() const;

private:
  struct Impl;
  Impl* impl_ = nullptr;
};

class FibonacciGetResultResponse
{
public:
  FibonacciGetResultResponse();
  ~FibonacciGetResultResponse();

  FibonacciGetResultResponse(const FibonacciGetResultResponse&) = delete;
  FibonacciGetResultResponse& operator=(const FibonacciGetResultResponse&) = delete;

  FibonacciGetResultResponse(FibonacciGetResultResponse&&) = delete;
  FibonacciGetResultResponse& operator=(FibonacciGetResultResponse&&) = delete;

  void set_status(int8_t status);
  void set_sequence_fibonacci(int32_t order);

  size_t sequence_length() const;
  int32_t sequence_at(size_t idx) const;

  void* ros_message();
  const void* ros_message() const;

private:
  struct Impl;
  Impl* impl_ = nullptr;
};

}  // namespace ros2_julia
