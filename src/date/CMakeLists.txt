# define a set of sources to compile
set(date_SOURCES
    time.cpp)

# export source files as library
add_library(transitdate STATIC
    ${date_SOURCES})

# link all required items
target_link_libraries(transitdate
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})

# define additional include directories
target_include_directories(transitdate SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
