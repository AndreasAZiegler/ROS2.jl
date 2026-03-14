cmake_minimum_required(VERSION 3.16)
project(ros2_julia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(ROS2_JULIA_BUILD_EXAMPLES "Build example binaries (optional)" OFF)

# ROS2 C API libs
find_package(rcl REQUIRED)
find_package(rcutils REQUIRED)
find_package(rmw REQUIRED)
find_package(rosidl_runtime_c REQUIRED)
find_package(rosidl_typesupport_c REQUIRED)

# For the proof-of-concept std_msgs/msg/String wrapper.
find_package(std_msgs REQUIRED)

# CxxWrap / libcxxwrap-julia (CMake package is typically JlCxx)
find_package(JlCxx REQUIRED)

# std_msgs exported targets vary by distro/build; capture them in variables.
set(_ROS2_JULIA_STD_MSGS_GEN_C std_msgs::std_msgs__rosidl_generator_c)
if(NOT TARGET ${_ROS2_JULIA_STD_MSGS_GEN_C})
  set(_ROS2_JULIA_STD_MSGS_GEN_C std_msgs__rosidl_generator_c)
endif()

set(_ROS2_JULIA_STD_MSGS_TS_C std_msgs::std_msgs__rosidl_typesupport_c)
if(NOT TARGET ${_ROS2_JULIA_STD_MSGS_TS_C})
  set(_ROS2_JULIA_STD_MSGS_TS_C std_msgs__rosidl_typesupport_c)
endif()

# Imported target names vary across ROS2/CMake setups.
set(_ROS2_JULIA_RCL_LIB rcl::rcl)
if(NOT TARGET ${_ROS2_JULIA_RCL_LIB})
  set(_ROS2_JULIA_RCL_LIB rcl)
endif()

set(_ROS2_JULIA_RCUTILS_LIB rcutils::rcutils)
if(NOT TARGET ${_ROS2_JULIA_RCUTILS_LIB})
  set(_ROS2_JULIA_RCUTILS_LIB rcutils)
endif()

set(_ROS2_JULIA_RMW_LIB rmw::rmw)
if(NOT TARGET ${_ROS2_JULIA_RMW_LIB})
  set(_ROS2_JULIA_RMW_LIB rmw)
endif()

set(_ROS2_JULIA_ROSIDL_RUNTIME_C_LIB rosidl_runtime_c::rosidl_runtime_c)
if(NOT TARGET ${_ROS2_JULIA_ROSIDL_RUNTIME_C_LIB})
  set(_ROS2_JULIA_ROSIDL_RUNTIME_C_LIB rosidl_runtime_c)
endif()

set(_ROS2_JULIA_ROSIDL_TYPESUPPORT_C_LIB rosidl_typesupport_c::rosidl_typesupport_c)
if(NOT TARGET ${_ROS2_JULIA_ROSIDL_TYPESUPPORT_C_LIB})
  set(_ROS2_JULIA_ROSIDL_TYPESUPPORT_C_LIB rosidl_typesupport_c)
endif()

add_library(ros2_julia SHARED
  cpp/module.cpp
  cpp/context.cpp
  cpp/node.cpp
  cpp/publisher.cpp
  cpp/subscription.cpp
  cpp/executor.cpp
  cpp/messages.cpp
  cpp/utils.cpp
)

target_include_directories(ros2_julia
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(ros2_julia
  PRIVATE
    JlCxx::cxxwrap_julia
    ${_ROS2_JULIA_RCL_LIB}
    ${_ROS2_JULIA_RCUTILS_LIB}
    ${_ROS2_JULIA_RMW_LIB}
    ${_ROS2_JULIA_ROSIDL_RUNTIME_C_LIB}
    ${_ROS2_JULIA_ROSIDL_TYPESUPPORT_C_LIB}
)

# std_msgs codegen + typesupport needed to compile and use StdMsgsString.
if(TARGET ${_ROS2_JULIA_STD_MSGS_GEN_C})
  target_link_libraries(ros2_julia PRIVATE ${_ROS2_JULIA_STD_MSGS_GEN_C})
endif()
if(TARGET ${_ROS2_JULIA_STD_MSGS_TS_C})
  target_link_libraries(ros2_julia PRIVATE ${_ROS2_JULIA_STD_MSGS_TS_C})
endif()

# Helpful on Linux when loading from Julia without installing system-wide.
#
# Embed ROS2 library locations into the RUNPATH so Julia can dlopen libros2_julia
# without requiring users to source setup scripts or set LD_LIBRARY_PATH.
if(UNIX AND NOT APPLE)
  set(_ros2_julia_rpath_dirs "")
  foreach(_t IN ITEMS
    ${_ROS2_JULIA_RCL_LIB}
    ${_ROS2_JULIA_RCUTILS_LIB}
    ${_ROS2_JULIA_RMW_LIB}
    ${_ROS2_JULIA_ROSIDL_RUNTIME_C_LIB}
    ${_ROS2_JULIA_ROSIDL_TYPESUPPORT_C_LIB}
    ${_ROS2_JULIA_STD_MSGS_TS_C}
  )
    if(TARGET ${_t})
      list(APPEND _ros2_julia_rpath_dirs "$<TARGET_FILE_DIR:${_t}>")
    endif()
  endforeach()

  list(REMOVE_DUPLICATES _ros2_julia_rpath_dirs)
  list(APPEND _ros2_julia_rpath_dirs "$ORIGIN")
  string(JOIN ";" _ros2_julia_rpath ${_ros2_julia_rpath_dirs})

  set_target_properties(ros2_julia PROPERTIES
    BUILD_RPATH "${_ros2_julia_rpath}"
    INSTALL_RPATH "${_ros2_julia_rpath}"
  )
endif()
