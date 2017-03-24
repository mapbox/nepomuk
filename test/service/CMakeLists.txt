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

add_unit_test(service_tile tile.cc "${serviceLIBS}" "${serviceINCLUDES}")
add_unit_test(service_master master.cc "${serviceLIBS}" "${serviceINCLUDES}")
