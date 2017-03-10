# define a set of sources to compile
set(search_SOURCES
    stop_to_line.cpp
    stop_to_line_factory.cpp)

# export source files as library
add_library(transitsearch STATIC
    ${search_SOURCES})

# link all required items
target_link_libraries(transitsearch
    ${Boost_LIBRARIES})

# define additional include directories
target_include_directories(transitsearch SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS})
