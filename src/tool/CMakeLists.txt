# define a set of sources to compile
SET(tool_SOURCES
                   )

# export source files as library
ADD_LIBRARY(tool, ${tool_SOURCES})

# link all required items
TARGET_LINK_LIBRARIES(tool,
    ${Boost_LIBRARIES})

# define additional include directories
TARGET_INCLUDE_DIRECTORIES(tool SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
