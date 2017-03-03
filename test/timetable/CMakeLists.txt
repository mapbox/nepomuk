set(timetableLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        transittimetable
        transitgtfs)

set(timetableINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(timetable_stop_table stop_table.cc "${timetableLIBS}" "${timetableINCLUDES}")

