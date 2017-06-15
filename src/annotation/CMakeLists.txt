# define a set of sources to compile
set(annotate_SOURCES
    api.cpp
    geometry.cpp
    geometry_factory.cpp
    pbf.cpp
    stop.cpp
    line.cpp
    line_factory.cpp)


# export source files as library
add_library(NEPOMUKannotate STATIC
    ${annotate_SOURCES})

# link all required items
target_link_libraries(NEPOMUKannotate
    ${Boost_LIBRARIES}
    NEPOMUKgeometric
    NEPOMUKipc
    NEPOMUKipcproto
    NEPOMUKproto)

# define additional include directories
target_include_directories(NEPOMUKannotate SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS})
