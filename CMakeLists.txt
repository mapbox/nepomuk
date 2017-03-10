cmake_minimum_required(VERSION 3.0.2)
cmake_policy(VERSION 3.0.2)
cmake_policy(SET CMP0037 NEW)

project(transit C CXX)

# Major for breaking ABI changes. Minor for features. Patch for bugfixes.
# see http://semver.org/ for context
set(TRANSIT_VERSION_MAJOR 0)
set(TRANSIT_VERSION_MINOR 0)
set(TRANSIT_VERSION_PATCH 0)

# fileformat versioning scheme
set(TRANSIT_FILEFORMAT_VERSION_MAJOR 0)
set(TRANSIT_FILEFORMAT_VERSION_MINOR 0)
set(TRANSIT_FILEFORMAT_VERSION_PATCH 0)

option(DEBUG_MODE "Compile in debug mode" OFF)
option(SANITIZE "Run adress sanitizer" OFF)
option(COVERAGE "Generate test converage data" OFF)

# include cmake modules from cmake folder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(directions_transit_os unix)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(directions_transit_os mac)
  link_directories(/usr/local/lib)
  include_directories(/usr/local/include)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message(FATAL_ERROR "Windows not yet supported")
  set(directions_transit_os win)

else()
  message(FATAL_ERROR "System not yet supported. Please open a ticket.")
endif()

# set compile flags based on optimisation / debug mode
add_compile_options(${CMAKE_CXX_FLAGS} -std=c++14)
add_compile_options(${CMAKE_CXX_FLAGS} -fvisibility=hidden)
add_compile_options(${CMAKE_CXX_FLAGS} -fPIC)
add_compile_options(${CMAKE_CXX_FLAGS} -Wall)
add_compile_options(${CMAKE_CXX_FLAGS} -Wextra)
add_compile_options(${CMAKE_CXX_FLAGS} -pedantic)
add_compile_options(${CMAKE_CXX_FLAGS} -Wuninitialized)
add_compile_options(${CMAKE_CXX_FLAGS} -Wunreachable-code)
add_compile_options(${CMAKE_CXX_FLAGS} -Wstrict-overflow=2)
add_compile_options(${CMAKE_CXX_FLAGS} -U_FORTIFY_SOURCE)
add_compile_options(${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2)

if(DEBUG_MODE)
    add_compile_options(${CMAKE_CXX_FLAGS} -fno-inline)
    add_compile_options(${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer)
    add_compile_options(${CMAKE_CXX_FLAGS} -g)
    add_compile_options(${CMAKE_CXX_FLAGS} -ggdb)
endif()

if(SANITIZE)
    add_compile_options(${CMAKE_CXX_FLAGS} -fsanitize=address)
    link_libraries(${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address)
    if(${directions_transit_os} MATCHES "unix")
        link_libraries(${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold)
    endif()
endif()

if(COVERAGE)
    if(!DEBUG_MODE)
        message(FATAL_ERROR "Configuration unreasonable: coverage requires debug mode.")
    endif()
    if(${directions_transit_os} MATCHES "unix")
        set(MAYBE_COVERAGE_LIBRARIES "-lgcov")
    elseif(${directions_transit_os} MATCHES "mac")
        set(MAYBE_COVERAGE_LIBRARIES "-fprofile-arcs -ftest-coverage")
    endif()

    add_compile_options(${CMAKE_CXX_FLAGS} -ftest-coverage)
    add_compile_options(${CMAKE_CXX_FLAGS} -fprofile-arcs)
endif()

#setup versioning scheme in header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/include/version.h)

include(mason)

option(USE_LTO "Use link-time optimization if compiler support can be detected" OFF)
set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION ${USE_LTO})

# third party libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Global include directory
include_directories("${PROJECT_SOURCE_DIR}/include")

# pull in mason dependencies
mason_use(libzmq VERSION 4.2.2)
mason_use(cppzmq VERSION 4.2.1 HEADER_ONLY)
set(Zmq_INCLUDE_DIRS
    ${MASON_PACKAGE_libzmq_INCLUDE_DIRS}
    ${MASON_PACKAGE_cppzmq_INCLUDE_DIRS})
set(Zmq_LIBRARIES
    ${MASON_PACKAGE_libzmq_STATIC_LIBS})

# add support for boost
set(BOOST_VERSION 1.63.0)
set(Boost_USE_STATIC_LIBS ON)
mason_use(boost VERSION ${BOOST_VERSION} HEADER_ONLY)
mason_use(boost_libfilesystem VERSION ${BOOST_VERSION})
#mason_use(boost_libiostreams VERSION ${BOOST_VERSION})
mason_use(boost_libprogram_options VERSION ${BOOST_VERSION})
#mason_use(boost_libregex VERSION ${BOOST_VERSION})
mason_use(boost_libtest VERSION ${BOOST_VERSION})
mason_use(boost_libdate_time VERSION ${BOOST_VERSION})
#mason_use(boost_libthread VERSION ${BOOST_VERSION})
mason_use(boost_libsystem VERSION ${BOOST_VERSION})

set(Boost_INCLUDE_DIRS ${MASON_PACKAGE_boost_INCLUDE_DIRS})
set(Boost_LIBRARIES
  ${MASON_PACKAGE_boost_libfilesystem_STATIC_LIBS}
#  ${MASON_PACKAGE_boost_libiostreams_STATIC_LIBS}
  ${MASON_PACKAGE_boost_libprogram_options_STATIC_LIBS}
#  ${MASON_PACKAGE_boost_libregex_STATIC_LIBS}
  ${MASON_PACKAGE_boost_libtest_STATIC_LIBS}
  ${MASON_PACKAGE_boost_libdate_time_STATIC_LIBS}
#  ${MASON_PACKAGE_boost_libthread_STATIC_LIBS}
  ${MASON_PACKAGE_boost_libsystem_STATIC_LIBS})

# required due to mason
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

# Optional Protobuf + ZeroMQ networking example
find_package(Protobuf 2.4.1 REQUIRED)

# Register sub-projects: provide CMakeLists.txt themselves with project specifics such as dependencies
# tool needs to be first to set proto headers
add_subdirectory(src/tool)

add_subdirectory(src/adaptor)
add_subdirectory(src/annotation)
add_subdirectory(src/geometric)
add_subdirectory(src/gtfs)
add_subdirectory(src/navigation)
add_subdirectory(src/timetable)


add_subdirectory(src/apps)

# testing, build unconditionally
enable_testing()
add_subdirectory(test)
