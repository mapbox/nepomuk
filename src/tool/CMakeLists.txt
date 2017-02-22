# define a set of sources to compile
SET(tool_SOURCES
    io/std_stream.cpp)

# export source files as library
ADD_LIBRARY(transittool
    ${tool_SOURCES})

# link all required items
TARGET_LINK_LIBRARIES(transittool
    ${Boost_LIBRARIES})

# define additional include directories
TARGET_INCLUDE_DIRECTORIES(transittool SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
