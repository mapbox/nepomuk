# define a set of sources to compile
set(service_SOURCES
    master.cpp
    route.cpp
    route_parameters.cpp
    tile_parameters.cpp
    tile.cpp)

# export source files as library
add_library(NEPOMUKservice STATIC
    ${service_SOURCES})

# link all required items
target_link_libraries(NEPOMUKservice
    ${Boost_LIBRARIES}
    NEPOMUKadaptor
    NEPOMUKannotate
    NEPOMUKtimetable
    NEPOMUKnavigation
    NEPOMUKsearch
    NEPOMUKproto
    NEPOMUKtool
    NEPOMUKdate
    NEPOMUKgtfs
    NEPOMUKalgorithm)
