# define a set of sources to compile
set(geometric_SOURCES
    bounding_box.cpp
    coordinate.cpp
    polyline.cpp)

# export source files as library
add_library(transitgeometric STATIC
    ${geometric_SOURCES})

# link all required items
target_link_libraries(transitgeometric
    ${Boost_LIBRARIES})

# define additional include directories
target_include_directories(transitgeometric SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS})
