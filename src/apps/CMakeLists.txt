set(EXEC_LIBS
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
    transitservice
    transittimetable)

set(EXEC_INCLUDES
    ${Boost_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
    ${LIBZMQ_INCLUDE_DIRS})

add_executable(csv-import csv-import.cpp)
target_link_libraries(csv-import ${EXEC_LIBS})
target_include_directories(csv-import SYSTEM PUBLIC ${EXEC_INCLUDES})

add_executable(tile tile.cpp)
target_link_libraries(tile ${EXEC_LIBS})
target_include_directories(tile SYSTEM PUBLIC ${EXEC_INCLUDES})
