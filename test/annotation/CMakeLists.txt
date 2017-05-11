set(annotationLIBS
        ${Boost_LIBRARIES}
        NEPOMUKannotate
        NEPOMUKgtfs
        NEPOMUKadaptor
        NEPOMUKtimetable
        NEPOMUKtool
        NEPOMUKgtfs
        NEPOMUKservice)

set(annotationINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(stop_info stop_info.cc "${annotationLIBS}" "${annotationINCLUDES}")
add_unit_test(geometry geometry.cc "${annotationLIBS}" "${annotationINCLUDES}")
add_unit_test(api api.cc "${annotationLIBS}" "${annotationINCLUDES}")
