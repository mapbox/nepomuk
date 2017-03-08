# define a set of sources to compile
SET(timetable_SOURCES
    stop_table.cpp
    stop_table_factory.cpp
    line_table.cpp
    departure_table.cpp
    departure_table_factory.cpp
    timetable_factory.cpp
    timetable.cpp)

# export source files as library
ADD_LIBRARY(transittimetable
    ${timetable_SOURCES})

# link all required items
TARGET_LINK_LIBRARIES(transittimetable
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})

# define additional include directories
TARGET_INCLUDE_DIRECTORIES(transittimetable SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
