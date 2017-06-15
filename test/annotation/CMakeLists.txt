set(annotationLIBS
        ${Boost_LIBRARIES}
        NEPOMUKannotate
        NEPOMUKgtfs
        NEPOMUKadaptor
        NEPOMUKtimetable
        NEPOMUKipc
        NEPOMUKipcproto
        NEPOMUKadaptor
        NEPOMUKtool
        NEPOMUKgtfs
        NEPOMUKservice)

set(annotationINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(stop stop.cc "${annotationLIBS}" "${annotationINCLUDES}")
add_unit_test(geometry geometry.cc "${annotationLIBS}" "${annotationINCLUDES}")
add_unit_test(api api.cc "${annotationLIBS}" "${annotationINCLUDES}")
add_unit_test(pbf pbf.cc "${annotationLIBS}" "${annotationINCLUDES}")
