include(${PROJECT_SOURCE_DIR}/cmake/protoc.cmake)

# define a set of sources to compile
PROTOBUF_GENERATE_CPP(local_proto_HEADERS proto_SOURCES
    proto/coordinate.proto
    proto/request.proto
    proto/response.proto
    proto/tile_request.proto
    proto/tile_response.proto
    proto/route_request.proto
    proto/route_response.proto)

# kept separate to allow `-w` as a compile option
add_library(NEPOMUKipcproto
    ${proto_SOURCES}
    ${local_proto_HEADERS})

target_link_libraries(NEPOMUKipcproto
    ${protobuf_LIBRARIES})

#since protoc generates a lot of warnings, we disable diagnostics for protoc files
target_compile_options(NEPOMUKipcproto PUBLIC -w)

set(ipc_SOURCES
    request_handler.cpp
    service.cpp
    service_configuration.cpp)

# export source files as library
add_library(NEPOMUKipc
    ${ipc_SOURCES})

# link all required items
target_link_libraries(NEPOMUKipc
    NEPOMUKipcproto
    NEPOMUKgeometric
    ${Zmq_LIBRARIES}
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})
