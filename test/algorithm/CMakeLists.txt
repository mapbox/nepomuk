set(algorithmLIBS
        ${Boost_LIBRARIES})

set(algorithmINCLUDES
        ${Boost_INCLUDE_DIRS})

add_unit_test(ranges ranges.cc "${algorithmLIBS}" "${algorithmINCLUDES}")
