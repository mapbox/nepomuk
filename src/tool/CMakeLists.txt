# define a set of sources to compile

PROTOBUF_GENERATE_CPP(proto_SOURCES proto_HEADERS
    proto/accessibility.proto
    proto/calendar.proto
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

ADD_LIBRARY(transitproto
    ${proto_SOURCES}
    ${proto_HEADERS})

#since protoc generates a lot of warnings, we disable diagnostics for protoc files
target_compile_options(transitproto PUBLIC -w)

SET(tool_SOURCES
    io/std_stream.cpp
    io/std_line_stream.cpp)

# export source files as library
ADD_LIBRARY(transittool
    ${tool_SOURCES})

# link all required items
TARGET_LINK_LIBRARIES(transittool
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})

# define additional include directories
TARGET_INCLUDE_DIRECTORIES(transittool SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
