set(annotationLIBS
        ${Boost_LIBRARIES}
        transitannotate
        transitgtfs
        transitadaptor
        transittimetable
        transittool
        transitgtfs)

set(annotationINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(annotation_stop_info stop_info.cc "${annotationLIBS}" "${annotationINCLUDES}")
