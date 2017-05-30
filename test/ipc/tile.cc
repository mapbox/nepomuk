// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"

#include "adaptor/ipc_response_to_json.hpp"
#include "ipc/request_handler.hpp"
#include "ipc/service.hpp"
#include "ipc/service_configuration.hpp"

#include "date/time.hpp"
#include "test/toolkit.hpp"

#include <string>

#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>

using namespace nepomuk;

const constexpr char *socket_id = "ipc:///tmp/nepomuk_ipc_test_socket_tile";

void run_service()
{
    ipc::ServiceConfiguration config("some_file", TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    ipc::Service ipc_service(config);
    ipc_service.listen(socket_id);
}

BOOST_AUTO_TEST_CASE(request_tile)
{
    boost::thread threadAlpha(run_service);

    ipc::RequestHandler handler(socket_id);

    service::TileParameters query(16384, 16383, 15);
    BOOST_CHECK(query.valid());

    auto query_pbf = handler(query);
    BOOST_CHECK(query_pbf.has_tile());

    adaptor::IPCResponseToJSON adaptor;
    auto json = adaptor(query_pbf);
    BOOST_CHECK(json.size() > 0);

    handler.send_shutdown();
    threadAlpha.join();
}
