#ifndef TRANSIT_TEST_TOOLKIT_HPP_
#define TRANSIT_TEST_TOOLKIT_HPP_

#include "geometric/coordinate.hpp"

transit::geometric::WGS84Coordinate make_coordinate(double lon, double lat)
{
    return {transit::geometric::makeLatLonFromDouble<transit::geometric::FixedLongitude>(lon),
            transit::geometric::makeLatLonFromDouble<transit::geometric::FixedLatitude>(lat)};
};

// this fixture is currently missing geometry for the polylines, needs to be fixed in the
// fixture itself. The same is true for the line names. When these features land, the fixture
// needs to be updated.
constexpr char const *const OSRM_ANNOTATED_QUERY_1_4 =
    "{\"code\": \"Ok\",\"routes\": [{\"distance\": 4,\"duration\": 1200,\"weight\": "
    "1200,\"weight_name\": \"duration\",\"geometry\": \"\",\"legs\":[{\"distance\": "
    "4,\"duration\": 1200,\"weight\": 1200,\"summary\": "
    "\"LINE_NAME_HERE\",\"steps\":[{\"geometry\": \"s]g^\",\"intersections\": [{\"location\": "
    "[0.005,0.0049]}],\"mode\": \"transit\",\"name\": \"Stop A Platform 2\",\"distance\": "
    "1,\"maneuver\":{\"location\": [0.005,0.0049],\"type\": \"depart\",\"modifier\": "
    "\"straight\"}},{\"geometry\": \"\",\"intersections\": [{\"location\": "
    "[0.005,0.0049]}],\"mode\": \"transit\",\"name\": \"Stop B\",\"distance\": "
    "1,\"maneuver\":{\"location\": [0.0152,0.005],\"type\": \"stay in vehicle\",\"modifier\": "
    "\"straight\"}},{\"geometry\": \"\",\"intersections\": [{\"location\": "
    "[0.005,0.0049]}],\"mode\": \"transit\",\"name\": \"Stop C\",\"distance\": "
    "1,\"maneuver\":{\"location\": [0.0253,0.005],\"type\": \"stay in vehicle\",\"modifier\": "
    "\"straight\"}},{\"geometry\": \"g^g|E\",\"intersections\": [{\"location\": "
    "[0.0354,0.005]}],\"mode\": \"transit\",\"name\": \"Stop D\",\"distance\": "
    "1,\"maneuver\":{\"location\": [0.0354,0.005],\"type\": \"arrive\",\"modifier\": "
    "\"straight\"}}]}]}],\"waypoints\":[{\"distance\": 0, \"name\": \"Stop A Platform 2\", "
    "\"location\" : [0.005,0.0049]},{\"distance\": 0, \"name\": \"Stop D\", \"location\" : "
    "[0.0354,0.005]}]}";

constexpr char const *const TRIP_ANNOTATED_QUERY_1_4 =
    "[Leg] Line: 1\n"
    "    1:00:00 at stop: 1 name: Stop A Platform 2 located at: 0.005 0.0049\n"
    "    0:00:00 at stop: 2 name: Stop B located at: 0.0152 0.005\n"
    "    0:00:00 at stop: 3 name: Stop C located at: 0.0253 0.005\n"
    "    1:30:00 at stop: 4 name: Stop D located at: 0.0354 0.005\n";
#endif // TRANSIT_TEST_TOOLKIT_HPP_
