# define a set of sources to compile
set(service_SOURCES
    master.cpp
    tile_parameters.cpp
    tile.cpp)

# export source files as library
add_library(transitservice STATIC
    ${service_SOURCES})

# link all required items
target_link_libraries(transitservice
    ${Boost_LIBRARIES}
    transitadaptor
    transitannotate
    transittimetable
    transitnavigation
    transitsearch
    transitproto
    transittool
    transitdate
    transitgtfs
    transitalgorithm)

# define additional include directories
target_include_directories(transitservice SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${protozero_INCLUDE_DIRS})
