#ifndef TRANSIT_TEST_TOOLKIT_HPP_
#define TRANSIT_TEST_TOOLKIT_HPP_

#include "geometric/coordinate.hpp"

transit::geometric::WGS84Coordinate make_coordinate(double lon, double lat)
{
    return {transit::geometric::makeLatLonFromDouble<transit::geometric::FixedLongitude>(lon),
            transit::geometric::makeLatLonFromDouble<transit::geometric::FixedLatitude>(lat)};
};

constexpr char const *const API_ANNOTATED_QUERY_1_4 =
    "{\"code\":\"Ok\",\"waypoints\":[[0.005,0.0049],[0.0354,0.005]],\"routes\":[{\"depart\":0,"
    "\"arrive\":5400,\"duration\":5400,\"summary\":\"most beautiful "
    "route\",\"legs\":[{\"depart\":0,\"arrive\":5400,\"duration\":5400,\"segments\":[{\"mode\":"
    "\"transit\",\"depart\":4200,\"arrive\":5400,\"duration\":1200,\"geometry\":\"s]g^Sw~@?c~@?c~@"
    "\",\"stops\":[{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":3600,\"depart\":4200},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":4200,\"depart\":4200},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":4200,\"depart\":4200},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":5400,\"depart\":5400}],\"connections\":[{\"depart\":4200,"
    "\"arrive\":5400,\"duration\":1200,\"name\":\"CONNECTION NAME HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"}]}]}]}]}";

constexpr char const *const API_ANNOTATED_QUERY_A_J =
    "{\"code\":\"Ok\",\"waypoints\":[[0.005,0.0049],[0.0555,0.206]],\"routes\":[{\"depart\":0,"
    "\"arrive\":18050,\"duration\":18050,\"summary\":\"most beautiful "
    "route\",\"legs\":[{\"depart\":0,\"arrive\":18050,\"duration\":18050,\"segments\":[{\"mode\":"
    "\"transit\",\"depart\":4200,\"arrive\":5400,\"duration\":1200,\"geometry\":\"s]g^Sw~@?c~@?c~@"
    "\",\"stops\":[{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":3622,\"depart\":4200},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":4200,\"depart\":4200},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":4200,\"depart\":4200},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":5400,\"depart\":5400}],\"connections\":[{\"depart\":4200,"
    "\"arrive\":5400,\"duration\":1200,\"name\":\"CONNECTION NAME HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"}]},{\"mode\":\"transfer\",\"depart\":12600,\"arrive\":18050,\"stopname\":\"Stopname "
    "will be here, believe "
    "me.\",\"geometry\":\"ofg@gyI??\",\"duration\":5450},{\"mode\":\"transit\",\"depart\":5400,"
    "\"arrive\":18050,\"duration\":12650,\"geometry\":\"ofg@gyI?S\",\"stops\":[{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":12600,\"depart\":18050},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":18050,\"depart\":18050}],\"connections\":[{\"depart\":"
    "5400,\"arrive\":18050,\"duration\":12650,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN HERE\"}]}]}]}]}";

#endif // TRANSIT_TEST_TOOLKIT_HPP_
