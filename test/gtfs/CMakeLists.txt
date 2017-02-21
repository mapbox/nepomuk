set(gtfsLIBS
        ${Boost_LIBRARIES}
        ${PROTOBUF_LITE_LIBRARIES}
        transittool
        transitgtfs)

set(gtfsINCLUDES
        ${Boost_INCLUDE_DIRS}
        ${PROTOBUF_INCLUDE_DIRS})

add_unit_test(gtfs_import_csv import-csv.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
add_unit_test(gtfs_date date.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
add_unit_test(gtfs_time time.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
add_unit_test(gtfs_accessibility accessibility.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
add_unit_test(gtfs_construct construct.cc "${gtfsLIBS}" "${gtfsINCLUDES}")
