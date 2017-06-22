cmake_minimum_required(VERSION 3.1)
cmake_policy(VERSION 3.1)
cmake_policy(SET CMP0037 NEW)

project(nepomuk C CXX)

message(STATUS "Compiler: ${CXX_COMPILER}")

# Major for breaking ABI changes. Minor for features. Patch for bugfixes.
# see http://semver.org/ for context
set(NEPOMUK_VERSION_MAJOR 0)
set(NEPOMUK_VERSION_MINOR 2)
set(NEPOMUK_VERSION_PATCH 0)
set(NEPOMUK_TAG -rc.0)

# fileformat versioning scheme
set(NEPOMUK_FILEFORMAT_VERSION_MAJOR 0)
set(NEPOMUK_FILEFORMAT_VERSION_MINOR 0)
set(NEPOMUK_FILEFORMAT_VERSION_PATCH 0)

option(SANITIZE "Run adress sanitizer" OFF)
option(COVERAGE "Generate test converage data" OFF)
option(NODEBINDINGS "Build node bindings" OFF)

# include cmake modules from cmake folder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(nepomuk_os unix)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(nepomuk_os mac)
#  link_directories(/usr/local/lib)
#  include_directories(SYSTEM AFTER /usr/local/include)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message(FATAL_ERROR "Windows not yet supported")
  set(nepomuk_os win)

else()
  message(FATAL_ERROR "System not yet supported. Please open a ticket.")
endif()

# set compile flags based on optimisation / debug mode
add_compile_options(${CMAKE_CXX_FLAGS} -std=c++14)
add_compile_options(${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden)
add_compile_options(${CMAKE_CXX_FLAGS} -fPIC)
add_compile_options(${CMAKE_CXX_FLAGS} -W)
add_compile_options(${CMAKE_CXX_FLAGS} -Wall)
add_compile_options(${CMAKE_CXX_FLAGS} -Wextra)
add_compile_options(${CMAKE_CXX_FLAGS} -pedantic)
add_compile_options(${CMAKE_CXX_FLAGS} -Wuninitialized)
add_compile_options(${CMAKE_CXX_FLAGS} -Wreturn-type)
add_compile_options(${CMAKE_CXX_FLAGS} -Wunreachable-code)
add_compile_options(${CMAKE_CXX_FLAGS} -Wstrict-overflow=2)
add_compile_options(${CMAKE_CXX_FLAGS} -U_FORTIFY_SOURCE)
add_compile_options(${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2)

if(nepomuk_os MATCHES "unix")
    add_compile_options(${CMAKE_CXX_FLAGS} -fext-numeric-literals)
endif()

# do not allow deprecrated calls
add_definitions(-DPROTOZERO_STRICT_API)


if(SANITIZE)
    add_compile_options(${CMAKE_CXX_FLAGS} -fsanitize=address)
    link_libraries(${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address)
    if(${nepomuk_os} MATCHES "unix")
        link_libraries(${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold)
    endif()
endif()

if(${nepomuk_os} MATCHES "unix")
    set(MAYBE_PHTREAD_LIBRARIES "-pthread")
endif()

if(COVERAGE)
    if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
        if(${nepomuk_os} MATCHES "unix")
            set(MAYBE_COVERAGE_LIBRARIES "-lgcov")
        elseif(${nepomuk_os} MATCHES "mac")
            set(MAYBE_COVERAGE_LIBRARIES "-fprofile-arcs -ftest-coverage")
        endif()
        add_compile_options(${CMAKE_CXX_FLAGS} -ftest-coverage)
        add_compile_options(${CMAKE_CXX_FLAGS} -fprofile-arcs)
    else()
        message(FATAL_ERROR "Configuration unreasonable: coverage requires debug mode.")
    endif()
endif()

#setup versioning scheme in header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/include/version.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/fixtures.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/include/fixtures.h)

if(${NODEBINDINGS})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/package.json.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/node/nepomuk/package.json)
endif()
include(mason)

option(USE_LTO "Use link-time optimization if compiler support can be detected" OFF)
set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION ${USE_LTO})

# third party libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Global include directory
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}")

# pull in mason dependencies
mason_use(libzmq VERSION 4.2.2)
mason_use(cppzmq VERSION 4.2.1 HEADER_ONLY)
set(Zmq_INCLUDE_DIRS
    ${MASON_PACKAGE_libzmq_INCLUDE_DIRS}
    ${MASON_PACKAGE_cppzmq_INCLUDE_DIRS})
set(Zmq_LIBRARIES
    ${MASON_PACKAGE_libzmq_STATIC_LIBS})

mason_use(protozero VERSION 1.5.1 HEADER_ONLY)
set(protozero_INCLUDE_DIRS
    ${MASON_PACKAGE_protozero_INCLUDE_DIRS})

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
mason_use(boost_libthread VERSION ${BOOST_VERSION})
mason_use(boost_libsystem VERSION ${BOOST_VERSION})

# handling timezone codes
mason_use(icu VERSION 58.1)
set(icu_INCLUDE_DIRS
    ${MASON_PACKAGE_icu_INCLUDE_DIRS})
set(icu_LIBRARIES
    ${MASON_PACKAGE_icu_STATIC_LIBS})

message(STATUS "ICU Libs: ${icu_LIBRARIES}")


set(Boost_INCLUDE_DIRS ${MASON_PACKAGE_boost_INCLUDE_DIRS})
set(Boost_LIBRARIES
  ${MASON_PACKAGE_boost_libfilesystem_STATIC_LIBS}
#  ${MASON_PACKAGE_boost_libiostreams_STATIC_LIBS}
  ${MASON_PACKAGE_boost_libprogram_options_STATIC_LIBS}
#  ${MASON_PACKAGE_boost_libregex_STATIC_LIBS}
  ${MASON_PACKAGE_boost_libtest_STATIC_LIBS}
  ${MASON_PACKAGE_boost_libdate_time_STATIC_LIBS}
  ${MASON_PACKAGE_boost_libthread_STATIC_LIBS}
  ${MASON_PACKAGE_boost_libsystem_STATIC_LIBS})

# required due to mason
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

mason_use(protobuf VERSION 3.3.0)
set(protobuf_LIBRARIES
  "${MASON_PACKAGE_protobuf_PREFIX}/lib/libprotobuf.a")
set(Mason_PROTOC
  "${MASON_PACKAGE_protobuf_PREFIX}/bin/protoc")

include_directories(SYSTEM BEFORE ${MASON_PACKAGE_protobuf_INCLUDE_DIRS})
include_directories(SYSTEM BEFORE ${protozero_INCLUDE_DIRS})
include_directories(SYSTEM BEFORE ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM BEFORE ${Zmq_INCLUDE_DIRS})
include_directories(SYSTEM BEFORE ${icu_INCLUDE_DIRS})

#for generated protobuf files
include_directories("${CMAKE_BINARY_DIR}/src")

# Register sub-projects: provide CMakeLists.txt themselves with project specifics such as dependencies
# tool needs to be first to set proto headers
add_subdirectory(src/tool)
add_subdirectory(src/ipc)

include_directories(PUBLIC ${PROTOBUF_INCLUDE_DIRS})

add_subdirectory(src/adaptor)
add_subdirectory(src/algorithm)
add_subdirectory(src/annotation)
add_subdirectory(src/date)
add_subdirectory(src/geometric)
add_subdirectory(src/gtfs)
add_subdirectory(src/navigation)
add_subdirectory(src/search)
add_subdirectory(src/service)
add_subdirectory(src/timetable)

if(${NODEBINDINGS})
    message(STATUS "running ${PROJECT_SOURCE_DIR}/scripts/configure-bindings.sh")
    execute_process(COMMAND ${PROJECT_SOURCE_DIR}/scripts/configure-bindings.sh
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    add_subdirectory(src/bindings)
endif()

add_subdirectory(src/apps)

# testing, build unconditionally
enable_testing()
add_subdirectory(test)
