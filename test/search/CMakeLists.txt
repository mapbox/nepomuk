set(searchLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        NEPOMUKsearch
        NEPOMUKservice
        NEPOMUKipc
        NEPOMUKipcproto
        NEPOMUKtimetable
        NEPOMUKgtfs)

set(searchINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(search_lookup_line stop_to_line.cc "${searchLIBS}" "${searchINCLUDES}")
add_unit_test(search_lookup_coordinates coordinate_to_stop.cc "${searchLIBS}" "${searchINCLUDES}")
