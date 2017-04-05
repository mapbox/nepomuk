set(toolLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        transittool
        transitgeometric)

set(toolINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(tool_io io.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(tool_id_hasher id_hasher.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(tool_decode_csv decode_csv.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(tool_container_dictonary dictonary.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(tool_surjective_mapping surjective_mapping.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(tool_graph graph.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(tool_vector_tile vector_tile.cc "${toolLIBS}" "${toolINCLUDES}")
