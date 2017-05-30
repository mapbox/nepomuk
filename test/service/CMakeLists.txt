set(serviceLIBS
        ${Boost_LIBRARIES}
        NEPOMUKadaptor
        NEPOMUKannotate
        NEPOMUKtimetable
        NEPOMUKnavigation
        NEPOMUKsearch
        NEPOMUKservice
        NEPOMUKipc
        NEPOMUKipcproto
        NEPOMUKproto
        NEPOMUKtool
        NEPOMUKgtfs)

set(serviceINCLUDES
        ${Boost_INCLUDE_DIRS})

add_unit_test(tile tile.cc "${serviceLIBS}" "${serviceINCLUDES}")
add_unit_test(route route.cc "${serviceLIBS}" "${serviceINCLUDES}")
add_unit_test(master master.cc "${serviceLIBS}" "${serviceINCLUDES}")
