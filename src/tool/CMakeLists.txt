include(${PROJECT_SOURCE_DIR}/cmake/protoc.cmake)

# define a set of sources to compile
PROTOBUF_GENERATE_CPP(local_proto_HEADERS proto_SOURCES
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

add_library(NEPOMUKproto
    ${proto_SOURCES}
    ${local_proto_HEADERS})

target_link_libraries(NEPOMUKproto
    ${protobuf_LIBRARIES})

#since protoc generates a lot of warnings, we disable diagnostics for protoc files
target_compile_options(NEPOMUKproto PUBLIC -w)

set(tool_SOURCES
    io/std_stream.cpp
    io/std_line_stream.cpp
    container/dictionary.cpp
    container/string_table.cpp
    container/mapbox_vector_tile.cpp
    status/progress.cpp)

# export source files as library
add_library(NEPOMUKtool
    ${tool_SOURCES})

# link all required items
target_link_libraries(NEPOMUKtool
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})
