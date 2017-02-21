cmake_minimum_required(VERSION 2.8.11)
project(directions_transit C CXX)


# Major for breaking ABI changes. Minor for features. Patch for bugfixes.
# see http://semver.org/ for context
set(SWEEP_VERSION_MAJOR 0)
set(SWEEP_VERSION_MINOR 0)
set(SWEEP_VERSION_PATCH 0)


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic -fvisibility=hidden -fPIC -fno-rtti")
  set(directions_transit_os unix)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic -fvisibility=hidden -fPIC -fno-rtti")
  set(directions_transit_os mac)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message(FATAL_ERROR "Windows not yet supported")
  set(directions_transit_os win)

else()
  message(FATAL_ERROR "System not yet supported. Please open a ticket.")
endif()

#setup versioning scheme in header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/include/transit/version.h)

# Register sub-projects: provide CMakeLists.txt themselves with project specifics such as dependencies
ADD_SUBDIRECTORY(tool/src)
ADD_SUBDIRECTORY(transit/src)
