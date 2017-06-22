set(dateLIBS
        ${Boost_LIBRARIES}
        ${icu_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        NEPOMUKdate)

set(dateINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(time time.cc "${dateLIBS}" "${dateINCLUDES}")
add_unit_test(timezone timezone.cc "${dateLIBS}" "${dateINCLUDES}")
add_unit_test(availability availability.cc "${dateLIBS}" "${dateINCLUDES}")
