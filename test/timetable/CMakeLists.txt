set(timetableLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        NEPOMUKservice
        NEPOMUKtimetable
        NEPOMUKgtfs)

set(timetableINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(timetable_stop_table stop_table.cc "${timetableLIBS}" "${timetableINCLUDES}")
add_unit_test(timetable_departure_table departure_table.cc "${timetableLIBS}" "${timetableINCLUDES}")
add_unit_test(timetable_transfer_table transfer_table.cc "${timetableLIBS}" "${timetableINCLUDES}")
add_unit_test(timetable_station_table station_table.cc "${timetableLIBS}" "${timetableINCLUDES}")
add_unit_test(timetable_graph_adaptor graph_adaptor.cc "${timetableLIBS}" "${timetableINCLUDES}")
