set(serviceLIBS
        ${Boost_LIBRARIES}
        transitadaptor
        transitannotate
        transittimetable
        transitnavigation
        transitsearch
        transitservice
        transitproto
        transittool
        transitgtfs)

set(serviceINCLUDES
        ${Boost_INCLUDE_DIRS})

add_unit_test(tile tile.cc "${serviceLIBS}" "${serviceINCLUDES}")
add_unit_test(earliest_arrival earliest_arrival.cc "${serviceLIBS}" "${serviceINCLUDES}")
add_unit_test(master master.cc "${serviceLIBS}" "${serviceINCLUDES}")
