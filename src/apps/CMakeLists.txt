set(EXEC_LIBS
	${Boost_LIBRARIES}
    ${Zmq_LIBRARIES}
    ${MAYBE_COVERAGE_LIBRARIES}
    NEPOMUKadaptor
    NEPOMUKannotate
    NEPOMUKproto
    NEPOMUKtool
    NEPOMUKipc
    NEPOMUKdate
    NEPOMUKgtfs
    NEPOMUKgeometric
    NEPOMUKnavigation
    NEPOMUKsearch
    NEPOMUKservice
    NEPOMUKtimetable)

set(EXEC_INCLUDES
    ${PROTOBUF_INCLUDE_DIRS})

add_executable(csv-import csv-import.cpp)
target_link_libraries(csv-import ${EXEC_LIBS})
target_include_directories(csv-import SYSTEM PUBLIC ${EXEC_INCLUDES})

add_executable(tile tile.cpp)
target_link_libraries(tile ${EXEC_LIBS})
target_include_directories(tile SYSTEM PUBLIC ${EXEC_INCLUDES})

add_executable(ipc-provider ipc-provider.cpp)
target_link_libraries(ipc-provider
                        ${EXEC_LIBS}
                        ${MAYBE_PHTREAD_LIBRARIES})
target_include_directories(ipc-provider SYSTEM PUBLIC ${EXEC_INCLUDES})
