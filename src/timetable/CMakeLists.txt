# define a set of sources to compile
SET(timetable_SOURCES
    stop_table.cpp
    stop_table_factory.cpp
    line_table.cpp
    departure_table.cpp
    departure_table_factory.cpp
    duration_table.cpp
    duration_table_factory.cpp
    graph_adaptor.cpp
    line_table.cpp
    line_table_factory.cpp
    station_table.cpp
    station_table_factory.cpp
    timetable_factory.cpp
    timetable.cpp
    transfer_table.cpp
    transfer_table_factory.cpp)

# export source files as library
ADD_LIBRARY(transittimetable
    ${timetable_SOURCES})

# link all required items
TARGET_LINK_LIBRARIES(transittimetable
    transitgeometric
    transitgtfs
    transitsearch
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})

# define additional include directories
TARGET_INCLUDE_DIRECTORIES(transittimetable SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
