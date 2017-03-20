# define a set of sources to compile
set(service_SOURCES
    tile_parameters.cpp
    tile.cpp)

# export source files as library
add_library(transitservice STATIC
    ${service_SOURCES})

# link all required items
target_link_libraries(transitservice
    ${Boost_LIBRARIES}
    transittimetable
    transitgtfs
    transitalgorithm)

# define additional include directories
target_include_directories(transitservice SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
