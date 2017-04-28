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
    "{\"code\": \"Ok\",\"routes\": [{\"distance\": 2,\"duration\": 5400,\"weight\": "
    "5400,\"weight_name\": \"duration\",\"geometry\": "
    "\"????s]g^Sw~@?c~@?c~@\",\"legs\":[{\"distance\": 2,\"duration\": 5400,\"weight\": "
    "5400,\"summary\": \"LINE_NAME_HERE\",\"steps\":[{\"geometry\": \"????\",\"mode\": "
    "\"walk\",\"distance\": 0,\"duration\": 0,\"maneuver\":{\"location\": [0,0],\"type\": "
    "\"walk\",\"modifier\": \"straight\"}},{\"geometry\": \"s]g^\",\"intersections\": "
    "[{\"location\": [0.005,0.0049]}],\"mode\": \"transit\",\"name\": \"Stop A Platform "
    "2\",\"distance\": 1,\"duration\": 0,\"maneuver\":{\"location\": [0.005,0.0049],\"type\": "
    "\"depart\",\"modifier\": \"straight\"}},{\"geometry\": \"g^_~A?c~@\",\"intersections\": "
    "[{\"location\": [0.005,0.0049]}],\"mode\": \"transit\",\"name\": \"Stop B\",\"distance\": "
    "1,\"duration\": 0,\"maneuver\":{\"location\": [0.0152,0.005],\"type\": \"stay in "
    "vehicle\",\"modifier\": \"straight\"}},{\"geometry\": \"g^c}C?c~@\",\"intersections\": "
    "[{\"location\": [0.005,0.0049]}],\"mode\": \"transit\",\"name\": \"Stop C\",\"distance\": "
    "1,\"duration\": 1200,\"maneuver\":{\"location\": [0.0253,0.005],\"type\": \"stay in "
    "vehicle\",\"modifier\": \"straight\"}},{\"geometry\": \"g^g|E\",\"intersections\": "
    "[{\"location\": [0.0354,0.005]}],\"mode\": \"transit\",\"name\": \"Stop D\",\"distance\": "
    "1,\"duration\": 0,\"maneuver\":{\"location\": [0.0354,0.005],\"type\": "
    "\"arrive\",\"modifier\": \"straight\"}}]}]}],\"waypoints\":[{\"distance\": 0, \"name\": "
    "\"UNKNOWN\", \"location\" : [0,0]},{\"distance\": 0, \"name\": \"Stop D\", \"location\" : "
    "[0.0354,0.005]}]}";

constexpr char const *const OSRM_ANNOTATED_QUERY_A_J =
    "{\"code\": \"Ok\",\"routes\": [{\"distance\": 4,\"duration\": 18050,\"weight\": "
    "18050,\"weight_name\": \"duration\",\"geometry\": "
    "\"????s]g^Sw~@?c~@?c~@S~|D??sff@_zH?S\",\"legs\":[{\"distance\": 4,\"duration\": "
    "18050,\"weight\": 18050,\"summary\": \"LINE_NAME_HERE\",\"steps\":[{\"geometry\": "
    "\"????\",\"mode\": \"walk\",\"distance\": 0,\"duration\": 0,\"maneuver\":{\"location\": "
    "[0,0],\"type\": \"walk\",\"modifier\": \"straight\"}},{\"geometry\": "
    "\"s]g^\",\"intersections\": [{\"location\": [0.005,0.0049]}],\"mode\": \"transit\",\"name\": "
    "\"Stop A Platform 2\",\"distance\": 1,\"duration\": 0,\"maneuver\":{\"location\": "
    "[0.005,0.0049],\"type\": \"depart\",\"modifier\": \"straight\"}},{\"geometry\": "
    "\"g^_~A?c~@\",\"intersections\": [{\"location\": [0.005,0.0049]}],\"mode\": "
    "\"transit\",\"name\": \"Stop B\",\"distance\": 1,\"duration\": 0,\"maneuver\":{\"location\": "
    "[0.0152,0.005],\"type\": \"stay in vehicle\",\"modifier\": \"straight\"}},{\"geometry\": "
    "\"g^c}C?c~@\",\"intersections\": [{\"location\": [0.005,0.0049]}],\"mode\": "
    "\"transit\",\"name\": \"Stop C\",\"distance\": 1,\"duration\": "
    "1200,\"maneuver\":{\"location\": [0.0253,0.005],\"type\": \"stay in vehicle\",\"modifier\": "
    "\"straight\"}},{\"geometry\": \"g^g|E\",\"intersections\": [{\"location\": "
    "[0.0354,0.005]}],\"mode\": \"transit\",\"name\": \"Stop D\",\"distance\": 1,\"duration\": "
    "0,\"maneuver\":{\"location\": [0.0354,0.005],\"type\": \"arrive\",\"modifier\": "
    "\"straight\"}},{\"geometry\": \"{^g^??\",\"intersections\": [{\"location\": "
    "[0.005,0.0051]}],\"mode\": \"walk\",\"name\": \"Stop A Platform 1\",\"distance\": "
    "0,\"duration\": 5450,\"maneuver\":{\"location\": [0.005,0.0051],\"type\": "
    "\"transfer\",\"modifier\": \"straight\"}},{\"geometry\": \"ofg@gyI\",\"intersections\": "
    "[{\"location\": [0.0554,0.206]}],\"mode\": \"transit\",\"name\": \"Stop H\",\"distance\": "
    "1,\"duration\": 0,\"maneuver\":{\"location\": [0.0554,0.206],\"type\": "
    "\"depart\",\"modifier\": \"straight\"}},{\"geometry\": \"ofg@{yI\",\"intersections\": "
    "[{\"location\": [0.0555,0.206]}],\"mode\": \"transit\",\"name\": \"Stop I\",\"distance\": "
    "1,\"duration\": 0,\"maneuver\":{\"location\": [0.0555,0.206],\"type\": "
    "\"arrive\",\"modifier\": \"straight\"}}]}]}],\"waypoints\":[{\"distance\": 0, \"name\": "
    "\"UNKNOWN\", \"location\" : [0,0]},{\"distance\": 0, \"name\": \"Stop I\", \"location\" : "
    "[0.0555,0.206]}]}";

constexpr char const *const API_ANNOTATED_QUERY_A_J =
    "{\"code\":\"Ok\",\"waypoints\":[[0,0],[0.0555,0.206]],\"routes\":[{\"depart\":0,\"arrive\":"
    "18050,\"duration\":18050,\"summary\":\"most beautiful "
    "route\",\"legs\":[{\"depart\":0,\"arrive\":18050,\"duration\":18050,\"segments\":[{\"mode\":"
    "\"walking\",\"depart\":4200,\"arrive\":4200,\"duration\":0,\"distance\":0,\"origin\":[0,0],"
    "\"destination\":[0,0]},{\"mode\":\"transit\",\"depart\":4200,\"arrive\":5400,\"duration\":"
    "1200,\"geometry\":\"POLYLINE HERE\",\"stops\":[{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":3622,\"depart\":4200},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":4200,\"depart\":4200},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":4200,\"depart\":4200},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":5400,\"depart\":5400}],\"connections\":[{\"depart\":4200,"
    "\"arrive\":5400,\"duration\":1200,\"name\":\"CONNECTION NAME HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"}]},{\"mode\":\"transfer\",\"depart\":12600,\"arrive\":18050,\"duration\":5450,"
    "\"origin\":{\"name\":\"NAME HERE\",\"location\":[0,0]},\"destination\":{\"name\":\"NAME "
    "HERE\",\"location\":[0,0]}},{\"mode\":\"transit\",\"depart\":5400,\"arrive\":18050,"
    "\"duration\":12650,\"geometry\":\"POLYLINE HERE\",\"stops\":[{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":12600,\"depart\":18050},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":18050,\"depart\":18050}],\"connections\":[{\"depart\":"
    "5400,\"arrive\":18050,\"duration\":12650,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN HERE\"}]}]}]}]}";

#endif // TRANSIT_TEST_TOOLKIT_HPP_
