set(navigationLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        transitnavigation
        transittimetable
        transitsearch
        transitgtfs)

set(navigationINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(navigation_find_route_timetable timetable.cc "${navigationLIBS}" "${navigationINCLUDES}")
