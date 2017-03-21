set(navigationLIBS
        ${Boost_LIBRARIES}
        transitadaptor
        transitannotate
        transittimetable
        transitsearch
        transitservice
        transitproto
        transittool
        transitgtfs)

set(navigationINCLUDES
        ${Boost_INCLUDE_DIRS})

add_unit_test(service_tile tile.cc "${navigationLIBS}" "${navigationINCLUDES}")
