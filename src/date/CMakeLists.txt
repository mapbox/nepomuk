# define a set of sources to compile
set(date_SOURCES
    availability.cpp
    time.cpp)

# export source files as library
add_library(NEPOMUKdate STATIC
    ${date_SOURCES})

# link all required items
target_link_libraries(NEPOMUKdate
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})

# define additional include directories
target_include_directories(NEPOMUKdate SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
