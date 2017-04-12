set(dateLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        transitdate)

set(dateINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(time time.cc "${dateLIBS}" "${dateINCLUDES}")
