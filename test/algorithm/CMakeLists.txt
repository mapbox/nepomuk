set(algorithmLIBS
        ${Boost_LIBRARIES}
        transittool
        transitalgorithm)

set(algorithmINCLUDES
        ${Boost_INCLUDE_DIRS})

add_unit_test(ranges ranges.cc "${algorithmLIBS}" "${algorithmINCLUDES}")
add_unit_test(scc scc.cc "${algorithmLIBS}" "${algorithmINCLUDES}")
