set(navigationLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        transitannotate
        transitnavigation
        transittimetable
        transitsearch
        transitgtfs
        transitservice
        transittool)

set(navigationINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(navigation_timetable timetable.cc "${navigationLIBS}" "${navigationINCLUDES}")
add_unit_test(navigation_dijkstra dijkstra.cc "${navigationLIBS}" "${navigationINCLUDES}")
