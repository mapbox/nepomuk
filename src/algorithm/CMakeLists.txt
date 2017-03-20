# define a set of sources to compile
set(algorithm_SOURCES
    scc.cpp)

# export source files as library
add_library(transitalgorithm STATIC
    ${algorithm_SOURCES})

# link all required items
target_link_libraries(transitalgorithm
    ${Boost_LIBRARIES}
    transitproto)

# define additional include directories
target_include_directories(transitalgorithm SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
