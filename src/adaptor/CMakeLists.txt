# define a set of sources to compile
set(adaptor_SOURCES
    dictionary.cpp)

# export source files as library
add_library(NEPOMUKadaptor STATIC
    ${adaptor_SOURCES})

# link all required items
target_link_libraries(NEPOMUKadaptor
    ${Boost_LIBRARIES}
    ${PROTOBUF_LITE_LIBRARIES}
    NEPOMUKproto)

# define additional include directories
target_include_directories(NEPOMUKadaptor SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS})
