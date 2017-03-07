set(algorithmLIBS
        ${Boost_LIBRARIES})

set(algorithmINCLUDES
        ${Boost_INCLUDE_DIRS})

add_unit_test(group_by group_by.cc "${algorithmLIBS}" "${algorithmINCLUDES}")
