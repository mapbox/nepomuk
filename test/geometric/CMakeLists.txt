set(geometricLIBS
        ${Boost_LIBRARIES}
        NEPOMUKgeometric)

set(geometricINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(coordinate coordinate.cc "${geometricLIBS}" "${geometricINCLUDES}")
add_unit_test(bounding_box bounding_box.cc "${geometricLIBS}" "${geometricINCLUDES}")
add_unit_test(polyline polyline.cc "${geometricLIBS}" "${geometricINCLUDES}")
