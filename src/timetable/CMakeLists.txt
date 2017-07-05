# define a set of sources to compile
SET(timetable_SOURCES
    graph_adaptor.cpp
    station_table.cpp
    station_table_factory.cpp
    timetable_factory.cpp
    timetable.cpp
    stop_to_trip.cpp
    trip_table.cpp
    trip_table_factory.cpp
    transfer_table.cpp
    transfer_table_factory.cpp)

# export source files as library
ADD_LIBRARY(NEPOMUKtimetable
    ${timetable_SOURCES})

# link all required items
TARGET_LINK_LIBRARIES(NEPOMUKtimetable
    NEPOMUKgeometric
    NEPOMUKgtfs
    NEPOMUKsearch
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})

# define additional include directories
TARGET_INCLUDE_DIRECTORIES(NEPOMUKtimetable SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
