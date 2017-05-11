# define a set of sources to compile
set(algorithm_SOURCES
    scc.cpp)

# export source files as library
add_library(NEPOMUKalgorithm STATIC
    ${algorithm_SOURCES})

# link all required items
target_link_libraries(NEPOMUKalgorithm
    ${Boost_LIBRARIES}
    NEPOMUKproto)

# define additional include directories
target_include_directories(NEPOMUKalgorithm SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
