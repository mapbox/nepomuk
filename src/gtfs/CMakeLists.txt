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
    trip.cpp)

# export source files as library
add_library(NEPOMUKgtfs STATIC
    ${gtfs_SOURCES})

# link all required items
target_link_libraries(NEPOMUKgtfs
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES}
    NEPOMUKtool
    NEPOMUKdate
    NEPOMUKgeometric)

# define additional include directories
target_include_directories(NEPOMUKgtfs SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
