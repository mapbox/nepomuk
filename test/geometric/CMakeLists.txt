set(geometricLIBS
        ${Boost_LIBRARIES}
        transitgeometric)

set(geometricINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(geometric_coordinate coordinate.cc "${geometricLIBS}" "${geometricINCLUDES}")
add_unit_test(geometric_bounding_box bounding_box.cc "${geometricLIBS}" "${geometricINCLUDES}")
