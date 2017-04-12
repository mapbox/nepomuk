#include "fixtures.h"
#include "geometric/coordinate.hpp"
#include "date/time.hpp"
#include "id/line.hpp"
#include "id/stop.hpp"
#include "navigation/algorithm/timetable.hpp"
#include "service/master.hpp"

#include <iostream>
#include <vector>

using namespace transit;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace
{
geometric::WGS84Coordinate make_coordinate(double lon, double lat)
{
    return geometric::WGS84Coordinate{
        geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
        geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat)};
}
}

BOOST_AUTO_TEST_CASE(annotate_osrm_trip)
{
    service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    navigation::algorithm::TimeTable timetable_router(data_service.timetable(),
                                                      data_service.stop_to_line());

    auto const &trip_annotation = data_service.trip_annotation();
    BOOST_CHECK_EQUAL(&trip_annotation, &data_service.trip_annotation());

    auto trip = timetable_router(date::Time("1:00:00"), StopID{1}, StopID{4});

    auto annotated_trip = trip_annotation(*trip);

    // this fixture is currently missing geometry for the polylines, needs to be fixed in the
    // fixture itself. The same is true for the line names. When these features land, the fixture
    // needs to be updated.
    auto const fixture =
        "[Leg] Line: 1\n"
        "	1:00:00 at stop: 1 name: Stop A Platform 2 located at: 0.005 0.0049\n"
        "	0:00:00 at stop: 2 name: Stop B located at: 0.0052 0.005\n"
        "	0:00:00 at stop: 3 name: Stop C located at: 0.0053 0.005\n"
        "	1:30:00 at stop: 4 name: Stop D located at: 0.0054 0.005\n";
    BOOST_CHECK(static_cast<bool>(trip));

    BOOST_CHECK_EQUAL(fixture, annotated_trip);
}
