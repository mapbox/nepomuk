# define a set of sources to compile
SET(navigation_SOURCES
    )
# export source files as library
ADD_LIBRARY(transitnavigation
    ${navigation_SOURCES})

# link all required items
TARGET_LINK_LIBRARIES(transitnavigation
    ${Boost_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES})

# define additional include directories
TARGET_INCLUDE_DIRECTORIES(transitnavigation SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
