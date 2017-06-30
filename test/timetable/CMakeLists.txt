set(timetableLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        NEPOMUKservice
        NEPOMUKtimetable
        NEPOMUKipc
        NEPOMUKipcproto
        NEPOMUKgtfs)

set(timetableINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(timetable_transfer_table transfer_table.cc "${timetableLIBS}" "${timetableINCLUDES}")
add_unit_test(timetable_station_table station_table.cc "${timetableLIBS}" "${timetableINCLUDES}")
add_unit_test(timetable_graph_adaptor graph_adaptor.cc "${timetableLIBS}" "${timetableINCLUDES}")
add_unit_test(timetable_trip_table trip_table.cc "${timetableLIBS}" "${timetableINCLUDES}")
