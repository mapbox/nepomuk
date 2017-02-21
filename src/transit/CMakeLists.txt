# define a set of sources to compile
SET(transit_SOURCES
                   )

# export source files as library
ADD_LIBRARY(transit, ${transit_SOURCES})

# link all required items
TARGET_LINK_LIBRARIES(transit,
    ${Boost_LIBRARIES})

# define additional include directories
TARGET_INCLUDE_DIRECTORIES(transit SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
