set(toolLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        transittool)

set(toolINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(tool_io io.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(tool_id ids.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(tool_id_hasher id_hasher.cc "${toolLIBS}" "${toolINCLUDES}")
add_unit_test(tool_decode_csv decode_csv.cc "${toolLIBS}" "${toolINCLUDES}")
