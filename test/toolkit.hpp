#ifndef NEPOMUK_TEST_TOOLKIT_HPP_
#define NEPOMUK_TEST_TOOLKIT_HPP_

#include "geometric/coordinate.hpp"

nepomuk::geometric::WGS84Coordinate make_coordinate(double lon, double lat)
{
    return {nepomuk::geometric::makeLatLonFromDouble<nepomuk::geometric::FixedLongitude>(lon),
            nepomuk::geometric::makeLatLonFromDouble<nepomuk::geometric::FixedLatitude>(lat)};
};

constexpr char const *const API_ANNOTATED_QUERY_1_4 =
    "{\"code\":\"Ok\",\"waypoints\":[[0.005,0.0049],[0.0354,0.005]],\"routes\":[{\"depart\":0,"
    "\"arrive\":6000,\"duration\":6000,\"summary\":\"most beautiful "
    "route\",\"legs\":[{\"depart\":0,\"arrive\":6000,\"duration\":6000,\"segments\":[{\"mode\":"
    "\"transit\",\"depart\":4200,\"arrive\":6000,\"duration\":1800,\"geometry\":\"g^g^?S?S?I?I?I?I?"
    "I?I\",\"stops\":[{\"name\":\"Stop A Platform "
    "2\",\"location\":[0,0],\"arrive\":3600,\"depart\":4200},{\"name\":\"Stop "
    "B\",\"location\":[0,0],\"arrive\":4800,\"depart\":4800},{\"name\":\"Stop "
    "C\",\"location\":[0,0],\"arrive\":5400,\"depart\":5400},{\"name\":\"Stop "
    "D\",\"location\":[0,0],\"arrive\":6000,\"depart\":6000}],\"connections\":[{\"depart\":4200,"
    "\"arrive\":6000,\"duration\":1800,\"name\":\"Horizontal Route "
    "Top\",\"short_name\":\"H1\",\"headsign\":\"ABCDE\"}]}]}]}]}";

constexpr char const *const API_ANNOTATED_QUERY_A_J =
    "{\"code\":\"Ok\",\"waypoints\":[[0.005,0.0049],[0.0756,0.206]],\"routes\":[{\"depart\":0,"
    "\"arrive\":21050,\"duration\":21050,\"summary\":\"most beautiful "
    "route\",\"legs\":[{\"depart\":0,\"arrive\":21050,\"duration\":21050,\"segments\":[{\"mode\":"
    "\"transit\",\"depart\":4200,\"arrive\":6000,\"duration\":1800,\"geometry\":\"g^g^?S?S?I?I?I?I?"
    "I?I\",\"stops\":[{\"name\":\"Stop A Platform "
    "2\",\"location\":[0,0],\"arrive\":3622,\"depart\":4200},{\"name\":\"Stop "
    "B\",\"location\":[0,0],\"arrive\":4800,\"depart\":4800},{\"name\":\"Stop "
    "C\",\"location\":[0,0],\"arrive\":5400,\"depart\":5400},{\"name\":\"Stop "
    "D\",\"location\":[0,0],\"arrive\":6000,\"depart\":6000}],\"connections\":[{\"depart\":4200,"
    "\"arrive\":6000,\"duration\":1800,\"name\":\"Horizontal Route "
    "Top\",\"short_name\":\"H1\",\"headsign\":\"ABCDE\"}]},{\"mode\":\"transfer\",\"depart\":6000,"
    "\"arrive\":13200,\"from_station\":\"Stop H\",\"to_station\":\"Stop "
    "H\",\"geometry\":\"ofg@gyI??\",\"duration\":7200},{\"mode\":\"transit\",\"depart\":18050,"
    "\"arrive\":21050,\"duration\":3000,\"geometry\":\"ofg@gyI?{E?kvB\",\"stops\":[{\"name\":"
    "\"Stop H\",\"location\":[0,0],\"arrive\":13200,\"depart\":18050},{\"name\":\"Stop "
    "I\",\"location\":[0,0],\"arrive\":20450,\"depart\":20450},{\"name\":\"Stop "
    "J\",\"location\":[0,0],\"arrive\":21050,\"depart\":21050}],\"connections\":[{\"depart\":18050,"
    "\"arrive\":21050,\"duration\":3000,\"name\":\"Horizontal Route "
    "Bottom\",\"short_name\":\"\",\"headsign\":\"HIJ\"}]}]}]}]}";

#endif // NEPOMUK_TEST_TOOLKIT_HPP_
