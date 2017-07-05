#include "annotation/pbf.hpp"
#include "adaptor/ipc_response_to_json.hpp"
#include "fixtures.h"
#include "service/master.hpp"
#include "test/mocks/navigation.hpp"
#include "test/toolkit.hpp"

#include <utility>
#include <vector>

#include <boost/optional.hpp>

using namespace nepomuk;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

const constexpr char *annotated_mock =
    "{\"code\":\"Ok\",\"routes\":[{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":\"0\","
    "\"legs\":[{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":\"0\",\"segments\":[{"
    "\"departure\":\"0\",\"arrival\":\"0\",\"duration\":\"0\",\"geometry\":\"????\",\"distance\":"
    "\"0\"},{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":\"0\",\"geometry\":\"g^g^\","
    "\"stops\":[{\"name\":\"Stop A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]},{\"name\":\"Stop "
    "A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]},{\"name\":\"Stop "
    "A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]}],\"connections\":["
    "{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"}]},{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":\"0\","
    "\"geometry\":\"{^g^??\",\"origin\":{\"name\":\"Stop A Platform "
    "1\",\"location\":[0.005000,0.005100]},\"destination\":{\"name\":\"Stop A Platform "
    "1\",\"location\":[0.005000,0.005100]}},{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":"
    "\"0\",\"geometry\":\"g^g^\",\"stops\":[{\"name\":\"Stop A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]},{\"name\":\"Stop "
    "A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]},{\"name\":\"Stop "
    "A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]}],\"connections\":["
    "{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"}]},{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":\"0\","
    "\"geometry\":\"????\",\"distance\":\"0\"}]},{\"departure\":\"0\",\"arrival\":\"0\","
    "\"duration\":\"0\",\"segments\":[{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":\"0\","
    "\"geometry\":\"????\",\"distance\":\"0\"},{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":"
    "\"0\",\"geometry\":\"g^g^\",\"stops\":[{\"name\":\"Stop A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]},{\"name\":\"Stop "
    "A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]},{\"name\":\"Stop "
    "A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]}],\"connections\":["
    "{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"}]},{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":\"0\","
    "\"geometry\":\"{^g^??\",\"origin\":{\"name\":\"Stop A Platform "
    "1\",\"location\":[0.005000,0.005100]},\"destination\":{\"name\":\"Stop A Platform "
    "1\",\"location\":[0.005000,0.005100]}},{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":"
    "\"0\",\"geometry\":\"g^g^\",\"stops\":[{\"name\":\"Stop A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]},{\"name\":\"Stop "
    "A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]},{\"name\":\"Stop "
    "A Platform "
    "1\",\"arrival\":\"0\",\"departure\":\"0\",\"location\":[0.005000,0.005100]}],\"connections\":["
    "{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"},{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"0\",\"arrival\":\"0\"}]},{\"departure\":\"0\",\"arrival\":\"0\",\"duration\":\"0\","
    "\"geometry\":\"????\",\"distance\":\"0\"}]}]}],\"waypoints\":[{\"coordinate\":[[0.000000,0."
    "000000]]},{\"coordinate\":[[0.000000,0.000000]]},{\"coordinate\":[[0.000000,0.000000]]}]}";

BOOST_AUTO_TEST_CASE(annotate_pbf_trip)
{
    service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    auto const &pbf = data_service.pbf_annotation();
    MockNavigator mocked_navigator(data_service.timetable());
    auto mocked_trip = mocked_navigator(date::Time("1:00:00"), StopID{0}, StopID{1});
    BOOST_CHECK(mocked_trip);

    if (mocked_trip)
    {
        std::vector<navigation::Route> routes;
        routes.push_back(*mocked_trip);

        ipc::RouteResponse result;
        pbf(result, routes);
        adaptor::IPCResponseToJSON to_json;
        auto json = to_json(result);
        BOOST_CHECK_EQUAL(json, annotated_mock);
    }
}
