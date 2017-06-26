# define a set of sources to compile
set(search_SOURCES
    coordinate_to_stop.cpp)

# export source files as library
add_library(NEPOMUKsearch STATIC
    ${search_SOURCES})

# link all required items
target_link_libraries(NEPOMUKsearch
    NEPOMUKgeometric
    ${Boost_LIBRARIES})

# define additional include directories
target_include_directories(NEPOMUKsearch SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS})
