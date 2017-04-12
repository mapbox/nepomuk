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
    "\"LINE_NAME_HERE\",\"steps\":[{\"geometry\": \"\",\"mode\": \"transit\",\"name\": \"Stop "
    "A Platform 2\",\"distance\": 1,\"maneuver\":{\"location\": [0.005,0.0049],\"type\": "
    "\"board\"}},{\"geometry\": \"\",\"mode\": \"transit\",\"name\": \"Stop B\",\"distance\": "
    "1,\"maneuver\":{\"location\": [0.0052,0.005],\"type\": \"stay in "
    "vehicle\"}},{\"geometry\": \"\",\"mode\": \"transit\",\"name\": \"Stop C\",\"distance\": "
    "1,\"maneuver\":{\"location\": [0.0053,0.005],\"type\": \"stay in vehicle\"}},{\"mode\": "
    "\"transit\",\"name\": \"Stop D\",\"distance\": 1,\"maneuver\":{\"location\": "
    "[0.0054,0.005],\"type\": \"deboard\"}}]}]}]}";

#endif // TRANSIT_TEST_TOOLKIT_HPP_
