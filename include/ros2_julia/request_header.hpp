#pragma once

#include <rmw/rmw.h>

#include <cstdint>
#include <cstring>

namespace ros2_julia
{

// Small wrapper for rmw_request_id_t so Julia can pass it around without knowing
// the struct layout.
class RequestHeader
{
public:
  RequestHeader()
  {
    std::memset(&id_, 0, sizeof(id_));
  }

  rmw_request_id_t* rmw_request_id()
  {
    return &id_;
  }

  const rmw_request_id_t* rmw_request_id() const
  {
    return &id_;
  }

private:
  rmw_request_id_t id_;
};

}  // namespace ros2_julia
