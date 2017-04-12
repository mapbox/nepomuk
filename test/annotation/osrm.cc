#include "fixtures.h"
#include "geometric/coordinate.hpp"
#include "date/time.hpp"
#include "id/line.hpp"
#include "id/stop.hpp"
#include "navigation/algorithm/timetable.hpp"
#include "service/master.hpp"
#include "test/toolkit.hpp"

#include <iostream>
#include <vector>

using namespace transit;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(annotate_osrm_trip)
{
    service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    navigation::algorithm::TimeTable timetable_router(data_service.timetable(),
                                                      data_service.stop_to_line());

    auto const &osrm_annotation = data_service.osrm_annotation();
    BOOST_CHECK_EQUAL(&osrm_annotation, &data_service.osrm_annotation());

    auto trip = timetable_router(date::Time("1:00:00"), StopID{1}, StopID{4});

    auto annotated_trip = osrm_annotation(*trip);

    BOOST_CHECK(static_cast<bool>(trip));

    BOOST_CHECK_EQUAL(OSRM_ANNOTATED_QUERY_1_4, annotated_trip);
}
