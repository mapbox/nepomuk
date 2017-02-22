cmake_minimum_required(VERSION 3.0.2)
cmake_policy(VERSION 3.0.2)
cmake_policy(SET CMP0037 NEW)

project(transit C CXX)


# Major for breaking ABI changes. Minor for features. Patch for bugfixes.
# see http://semver.org/ for context
set(TRANSIT_VERSION_MAJOR 0)
set(TRANSIT_VERSION_MINOR 0)
set(TRANSIT_VERSION_PATCH 0)


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic -fvisibility=hidden -fPIC -fno-rtti")
  set(directions_transit_os unix)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic -fvisibility=hidden -fPIC -fno-rtti")
  set(directions_transit_os mac)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message(FATAL_ERROR "Windows not yet supported")
  set(directions_transit_os win)

else()
  message(FATAL_ERROR "System not yet supported. Please open a ticket.")
endif()

#setup versioning scheme in header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/include/version.h)


option(USE_LTO "Use link-time optimization if compiler support can be detected" ON)
set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION ${USE_LTO})

# third party libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Global include directory
include_directories("${PROJECT_SOURCE_DIR}/include")

# add support for boost
find_package(Boost 1.61.0 REQUIRED COMPONENTS filesystem system program_options thread timer log log_setup unit_test_framework)
add_definitions(-DBOOST_ALL_DYN_LINK)
add_definitions(-DBOOST_FILESYSTEM_NO_DEPRECATED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

# Optional Protobuf + ZeroMQ networking example
find_package(Protobuf 2.4.1 REQUIRED)

include(FindPkgConfig)
pkg_check_modules(LIBZMQ libzmq)

if (PROTOBUF_FOUND AND LIBZMQ_FOUND)
  #protobuf_generate_cpp(ProtoSources ProtoHeaders net.proto)
  set(LIBZMQ_INCLUDE_DIR ${LIBZMQ_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/third_party/zmqpp/)
endif()

# Register sub-projects: provide CMakeLists.txt themselves with project specifics such as dependencies
add_subdirectory(src/tool)
# add_subdirectory(src/transit)
add_subdirectory(src/gtfs)

# testing, build unconditionally
enable_testing()
add_subdirectory(test)
