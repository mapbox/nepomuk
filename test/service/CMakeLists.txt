set(navigationLIBS
        ${Boost_LIBRARIES}
        transittimetable
        transitsearch
        transitservice
        transitgtfs)

set(navigationINCLUDES
        ${Boost_INCLUDE_DIRS})

add_unit_test(service_tile tile.cc "${navigationLIBS}" "${navigationINCLUDES}")
