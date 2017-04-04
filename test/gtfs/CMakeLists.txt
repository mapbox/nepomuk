set(gtfsLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        transittool
        transitgeometric
        transitgtfs)

set(gtfsINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(import_csv import-csv.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
add_unit_test(date date.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
add_unit_test(time time.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
add_unit_test(accessibility accessibility.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
add_unit_test(construct construct.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
add_unit_test(dataset dataset.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
