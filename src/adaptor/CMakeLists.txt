# define a set of sources to compile
set(adaptor_SOURCES
    dictionary.cpp)

# export source files as library
add_library(transitadaptor STATIC
    ${adaptor_SOURCES})

# link all required items
target_link_libraries(transitadaptor
    ${Boost_LIBRARIES}
    ${PROTOBUF_LITE_LIBRARIES}
    transitproto)

# define additional include directories
target_include_directories(transitadaptor SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS})
