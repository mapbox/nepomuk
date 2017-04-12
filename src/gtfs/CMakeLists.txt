# define a set of sources to compile
set(gtfs_SOURCES
    read_csv.cpp
    accessibility.cpp
    agency.cpp
    calendar.cpp
    dataset.cpp
    fare.cpp
    date.cpp
    feed_info.cpp
    frequency.cpp
    route.cpp
    shape.cpp
    schedule.cpp
    stop.cpp
    transfer.cpp
    trip.cpp
    provider.cpp)

# export source files as library
add_library(transitgtfs STATIC
    ${gtfs_SOURCES})

# link all required items
target_link_libraries(transitgtfs
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES}
    transittool
    transitdate
    transitgeometric)

# define additional include directories
target_include_directories(transitgtfs SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${Zmq_INCLUDE_DIRS})
