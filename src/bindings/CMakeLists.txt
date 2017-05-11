# define a set of sources to compile
project(nepomuk VERSION ${NEPOMUK_VERSION_MAJOR}.${NEPOMUK_VERSION_MINOR}.${NEPOMUK_VERSION_PATCH} LANGUAGES C CXX)

message(STATUS "NodeCMAKE: ${CMAKE_SOURCE_DIR}/node_modules/node-cmake/")

set(NodeJS_DOWNLOAD ON CACHE INTERNAL "Download node.js sources" FORCE)
set(NodeJS_USE_CLANG_STDLIB OFF CACHE BOOL "Don't use libc++ by default" FORCE)

include(${CMAKE_SOURCE_DIR}/NodeJS.cmake)

set(binding_directory ${CMAKE_SOURCE_DIR}/node/nepomuk/binding)

# Note: although we set the global property, we need this for CMake to specify -std=c++14
nodejs_init()
add_nodejs_module(${PROJECT_NAME} node.cpp)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC "src"
    ${Boost_INCLUDE_DIRS}
    ${protozero_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    NEPOMUKadaptor
    NEPOMUKalgorithm
    NEPOMUKannotate
    NEPOMUKdate
    NEPOMUKgtfs
    NEPOMUKtimetable
    NEPOMUKproto
    NEPOMUKservice
    NEPOMUKtool)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)

file(MAKE_DIRECTORY ${binding_directory})

add_custom_command(OUTPUT ${binding_directory}/${PROJECT_NAME}.node
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${binding_directory}
                   DEPENDS ${PROJECT_NAME} ${binding_directory})

add_custom_target(copy_bindings ALL DEPENDS "${binding_directory}/${PROJECT_NAME}.node")
