set(toolLIBS
    ${Boost_LIBRARIES})

set(toolINCLUDES
    ${Boost_INCLUDE_DIRS})

add_unit_test(id ids.cc "${toolLIBS}" "${toolINCLUDES}")
