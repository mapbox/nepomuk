set(toolLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        transittool
        transitgeometric)

set(toolINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(io io.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(id_hasher id_hasher.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(decode_csv decode_csv.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(container_dictonary dictonary.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(surjective_mapping surjective_mapping.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(graph graph.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(vector_tile vector_tile.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(indexed_vector indexed_vector.cc "${toolLIBS}" "${toolINCLUDES}")
