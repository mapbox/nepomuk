# define a set of sources to compile
set(gtfs_SOURCES
    gtfs_reader.cpp)

# export source files as library
add_library(transitgtfs STATIC
    ${gtfs_SOURCES})

# link all required items
target_link_libraries(transitgtfs
    ${Boost_LIBRARIES}
    ${PROTOBUF_LITE_LIBRARIES}
    ${Zmq_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})

# define additional include directories
target_include_directories(transitgtfs SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
    ${Zmq_INCLUDE_DIRS})
