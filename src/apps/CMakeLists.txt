set(gtfsprovider_SOURCES
    csv-import.cpp)

add_executable(csv-import
    ${gtfsprovider_SOURCES})

target_link_libraries(csv-import
    ${Boost_LIBRARIES}
    ${LIBZMQ_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES}
    transitadaptor
    transitannotate
    transitproto
    transittool
    transitgtfs
    transitgeometric
    transitnavigation
    transitsearch
    transittimetable)

target_include_directories(csv-import SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
    ${LIBZMQ_INCLUDE_DIRS})
