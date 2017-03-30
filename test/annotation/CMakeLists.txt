set(annotationLIBS
        ${Boost_LIBRARIES}
        transitannotate
        transitgtfs
        transitadaptor
        transittimetable
        transittool
        transitgtfs
        transitservice)

set(annotationINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(stop_info stop_info.cc "${annotationLIBS}" "${annotationINCLUDES}")
add_unit_test(geometry geometry.cc "${annotationLIBS}" "${annotationINCLUDES}")
add_unit_test(osrm osrm.cc "${annotationLIBS}" "${annotationINCLUDES}")
add_unit_test(trip trip.cc "${annotationLIBS}" "${annotationINCLUDES}")
