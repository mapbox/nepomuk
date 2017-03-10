# define a set of sources to compile
set(annotate_SOURCES
    stop_info.cpp
    trip.cpp)

# export source files as library
add_library(transitannotate STATIC
    ${annotate_SOURCES})

# link all required items
target_link_libraries(transitannotate
    ${Boost_LIBRARIES}
    transitproto)

# define additional include directories
target_include_directories(transitannotate SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS})
