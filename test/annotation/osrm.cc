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

    auto const &osrm_annotation = data_service.osrm_annotation();
    BOOST_CHECK_EQUAL(&osrm_annotation, &data_service.osrm_annotation());

    auto trip = timetable_router(date::Time("1:00:00"), StopID{1}, StopID{4});

    auto annotated_trip = osrm_annotation(*trip);

    // this fixture is currently missing geometry for the polylines, needs to be fixed in the
    // fixture itself. The same is true for the line names. When these features land, the fixture
    // needs to be updated.
    auto const fixture =
        "{\"code\": \"Ok\",\"routes\": [{\"distance\": 4,\"duration\": 1200,\"weight\": "
        "1200,\"weight_name\": \"duration\",\"geometry\": \"\",\"legs\":[{\"distance\": "
        "4,\"duration\": 1200,\"weight\": 1200,\"summary\": "
        "\"LINE_NAME_HERE\",\"steps\":[{\"geometry\": \"\",\"mode\": \"transit\",\"name\": \"Stop "
        "A Platform 2\",\"distance\": 1,\"maneuver\":{\"location\": [0.005,0.0049],\"type\": "
        "\"board\"}},{\"geometry\": \"\",\"mode\": \"transit\",\"name\": \"Stop B\",\"distance\": "
        "1,\"maneuver\":{\"location\": [0.0052,0.005],\"type\": \"stay in "
        "vehicle\"}},{\"geometry\": \"\",\"mode\": \"transit\",\"name\": \"Stop C\",\"distance\": "
        "1,\"maneuver\":{\"location\": [0.0053,0.005],\"type\": \"stay in vehicle\"}},{\"mode\": "
        "\"transit\",\"name\": \"Stop D\",\"distance\": 1,\"maneuver\":{\"location\": "
        "[0.0054,0.005],\"type\": \"deboard\"}}]}]}]}";

    BOOST_CHECK(static_cast<bool>(trip));

    BOOST_CHECK_EQUAL(fixture, annotated_trip);
}
