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


OPTION(USE_LTO "Use link-time optimization if compiler support can be detected" ON)
SET_PROPERTY(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION ${USE_LTO})

# third party libraries
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Global include directory
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")

# add support for boost
FIND_PACKAGE(Boost 1.59.0 REQUIRED COMPONENTS filesystem system program_options thread timer log log_setup unit_test_framework)
ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
ADD_DEFINITIONS(-DBOOST_FILESYSTEM_NO_DEPRECATED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

# Optional Protobuf + ZeroMQ networking example
find_package(Protobuf 2.4.1 REQUIRED)

include(FindPkgConfig)
pkg_check_modules(LIBZMQ libzmq)

if (PROTOBUF_FOUND AND LIBZMQ_FOUND)
  #protobuf_generate_cpp(ProtoSources ProtoHeaders net.proto)
endif()

# Register sub-projects: provide CMakeLists.txt themselves with project specifics such as dependencies
# ADD_SUBDIRECTORY(src/tool)
# ADD_SUBDIRECTORY(src/transit)
ADD_SUBDIRECTORY(src/gtfs)
