# define a set of sources to compile
PROTOBUF_GENERATE_CPP(proto_SOURCES local_proto_HEADERS
    proto/accessibility.proto
    proto/calendar.proto
    proto/dictionary.proto
    proto/fare.proto
    proto/gtfs.proto
    proto/schedule.proto
    proto/stop.proto
    proto/trip.proto
    proto/agency.proto
    proto/coordinate.proto
    proto/frequency.proto
    proto/route.proto
    proto/shape.proto
    proto/transfer.proto)

add_library(transitproto
    ${proto_SOURCES}
    ${local_proto_HEADERS})

target_link_libraries(transitproto
    ${PROTOBUF_LIBRARIES})

# add protobuf include dir to parent scope
set(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

#since protoc generates a lot of warnings, we disable diagnostics for protoc files
target_compile_options(transitproto PUBLIC -w)

#pass on variables to parent scope
set(proto_HEADERS ${local_proto_HEADERS} PARENT_SCOPE)

set(tool_SOURCES
    io/std_stream.cpp
    io/std_line_stream.cpp
    container/dictionary.cpp
    container/string_table.cpp)

# export source files as library
add_library(transittool
    ${tool_SOURCES})

# link all required items
target_link_libraries(transittool
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})

# define additional include directories
TARGET_INCLUDE_DIRECTORIES(transittool SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
