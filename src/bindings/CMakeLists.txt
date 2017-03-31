# define a set of sources to compile
set(binding_SOURCES
    node.cpp)

project(transit VERSION ${TRANSIT_VERSION_MAJOR}.${TRANSIT_VERSION_MINOR}.${TRANSIT_VERSION_PATCH} LANGUAGES C CXX)

message(STATUS "NodeCMAKE: ${CMAKE_SOURCE_DIR}/node_modules/node-cmake/")

set(NodeJS_CXX_STANDARD 14 CACHE INTERNAL "Use C++14" FORCE)
set(NodeJS_DOWNLOAD ON CACHE INTERNAL "Download node.js sources" FORCE)
set(NodeJS_USE_CLANG_STDLIB OFF CACHE BOOL "Don't use libc++ by default" FORCE)
include(${CMAKE_SOURCE_DIR}/NodeJS.cmake)

# Note: although we set the global property, we need this for CMake to specify -std=c++14
nodejs_init()
add_nodejs_module(${PROJECT_NAME}
	${binding_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC "src"
    ${Boost_INCLUDE_DIRS}
    ${protozero_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    transitadaptor
    transitalgorithm
    transitannotate
    transitgtfs
    transittimetable
    transitproto
    transitservice
    transittool)


MESSAGE(STATUS "Node.js include path: ${NodeJS_INCLUDE_PATHS}")
MESSAGE(STATUS "Node.js libraries: ${NodeJS_LIBRARIES}")