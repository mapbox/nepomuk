// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"

#include "ipc/request_handler.hpp"
#include "ipc/service.hpp"

#include "ipc/service_configuration.hpp"

#include "date/time.hpp"
#include "test/toolkit.hpp"

#include <string>

#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>

#include "adaptor/ipc_response_to_json.hpp"

using namespace nepomuk;

const constexpr char *socket_id = "ipc:///tmp/nepomuk_ipc_route";

const constexpr char *annotated_route =
    "{\"code\":\"Ok\",\"routes\":[{\"departure\":\"0\",\"arrival\":\"5400\",\"duration\":\"5400\","
    "\"legs\":[{\"departure\":\"0\",\"arrival\":\"5400\",\"duration\":\"5400\",\"segments\":[{"
    "\"departure\":\"4200\",\"arrival\":\"5400\",\"duration\":\"1200\",\"geometry\":\"s]g^Sw~@?c~@?"
    "c~@\",\"stops\":[{\"name\":\"Stop A Platform "
    "2\",\"arrival\":\"3600\",\"departure\":\"4200\",\"location\":[0.005000,0.004900]},{\"name\":"
    "\"Stop "
    "B\",\"arrival\":\"4200\",\"departure\":\"4200\",\"location\":[0.015200,0.005000]},{\"name\":"
    "\"Stop "
    "C\",\"arrival\":\"4200\",\"departure\":\"4200\",\"location\":[0.025300,0.005000]},{\"name\":"
    "\"Stop "
    "D\",\"arrival\":\"5400\",\"departure\":\"5400\",\"location\":[0.035400,0.005000]}],"
    "\"connections\":[{\"headsign\":\"ABCDE\",\"name\":\"Horizontal Route "
    "Top\",\"name_color_text\":\"#000000\",\"name_color_background\":\"#FF0000\",\"departure\":"
    "\"4200\",\"arrival\":\"4200\"}]}]}]}],\"waypoints\":[{\"coordinate\":[[0.005000,0.004900]]},{"
    "\"coordinate\":[[0.035400,0.005000]]}]}";

void run_service()
{
    ipc::ServiceConfiguration config("some_file", TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    ipc::Service ipc_service(config);

    std::cout << "Listening" << std::endl;
    ipc_service.listen(socket_id);
}

BOOST_AUTO_TEST_CASE(request_route)
{
    //  Prepare our context and socket
    boost::thread threadAlpha(run_service);

    ipc::RequestHandler handler(socket_id);

    auto alive = handler.check_alive();
    BOOST_CHECK(alive.has_code());
    BOOST_CHECK_EQUAL(alive.code(), 200);
    BOOST_CHECK(alive.has_code_message());
    BOOST_CHECK_EQUAL(alive.code_message(), "I am awake!");

    date::UTCTimestamp april_thirteenth_one_am(1492045200);
    auto const stop_a1 = make_coordinate(0.005, 0.0049);
    auto const stop_d = make_coordinate(0.0353, 0.005);

    service::RouteParameters depart(
        stop_a1, stop_d, april_thirteenth_one_am, boost::none, 150.0, 1.0, 1.0);
    BOOST_CHECK(depart.valid());
    auto depart_pbf = handler(depart);
    BOOST_CHECK(depart_pbf.has_route());

    adaptor::IPCResponseToJSON adaptor;
    auto json = adaptor(depart_pbf);

    BOOST_CHECK_EQUAL(json, annotated_route);

    service::RouteParameters arrive(
        stop_a1, stop_d, boost::none, april_thirteenth_one_am, 150.0, 1.0, 1.0);
    BOOST_CHECK(arrive.valid());

    // we do not support requests yet
    BOOST_CHECK_THROW(handler(arrive), std::runtime_error);

    handler.send_shutdown();

    threadAlpha.join();
}
