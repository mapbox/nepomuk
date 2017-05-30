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

#include <stdexcept>

#include <zmq.hpp>

#include "adaptor/ipc_response_to_json.hpp"

using namespace nepomuk;

// return a predetermined message
struct SpecialService
{
    std::string buffer;
    std::string socket_id;
    SpecialService(std::string const &buffer, std::string const &socket_id)
        : buffer(buffer), socket_id(socket_id)
    {
    }

    void operator()()
    {
        int constexpr num_threads = 1;
        zmq::context_t context(num_threads);
        zmq::socket_t socket(context, ZMQ_REP);

        // listen on the requested port
        socket.bind(socket_id);

        auto const send_buffered_response = [&](std::string const &buffer) {
            zmq::message_t zmq_response(buffer.size());
            std::copy(buffer.begin(), buffer.end(), static_cast<char *>(zmq_response.data()));
            socket.send(zmq_response);
        };

        while (true)
        {
            zmq::message_t zmq_request;

            //  Wait for next request from client
            socket.recv(&zmq_request);

            nepomuk::ipc::Request pbf_request;

            std::string data;
            data.resize(zmq_request.size());
            std::copy(static_cast<const char *>(zmq_request.data()),
                      static_cast<const char *>(zmq_request.data()) + zmq_request.size(),
                      data.begin());
            // parse a request from the socket
            if (pbf_request.ParseFromString(data))
            {
                // allow shutting down the server
                if (pbf_request.has_kill_switch())
                {
                    if (pbf_request.kill_switch() == true)
                        break;
                }
                send_buffered_response(buffer);
            }
            else
            {
                // received an invalid or incomplete message
                send_buffered_response(buffer);
            }
        }
    }
};

BOOST_AUTO_TEST_CASE(empty_route)
{
    const constexpr char *socket_id = "ipc:///tmp/nepomuk_empty_tile";
    ipc::Response response;
    std::string buffer;
    response.SerializeToString(&buffer);

    SpecialService service(buffer, socket_id);
    boost::thread threadAlpha(service);

    ipc::RequestHandler handler(socket_id);

    date::UTCTimestamp april_thirteenth_one_am(1492045200);
    auto const stop_a1 = make_coordinate(0.005, 0.0049);
    auto const stop_d = make_coordinate(0.0353, 0.005);

    service::RouteParameters parameters(
        stop_a1, stop_d, april_thirteenth_one_am, boost::none, 150.0, 1.0, 1.0);
    BOOST_CHECK_THROW(handler(parameters), std::runtime_error);

    handler.send_shutdown();
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(empty_tile_request)
{
    const constexpr char *socket_id = "ipc:///tmp/nepomuk_empty_tile";
    ipc::Response response;
    std::string buffer;
    response.SerializeToString(&buffer);

    SpecialService service(buffer, socket_id);
    boost::thread threadAlpha(service);

    ipc::RequestHandler handler(socket_id);

    service::TileParameters invalid_parameters(0, 0, 12);
    BOOST_CHECK_THROW(handler(invalid_parameters), std::runtime_error);

    handler.send_shutdown();
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(tile_without_result)
{
    const constexpr char *socket_id = "ipc:///tmp/tile_without_result";
    ipc::Response response;
    response.set_code(200);
    response.mutable_tile();
    std::string buffer;
    response.SerializeToString(&buffer);

    SpecialService service(buffer, socket_id);
    boost::thread threadAlpha(service);

    ipc::RequestHandler handler(socket_id);

    service::TileParameters parameters(0, 0, 12);
    adaptor::IPCResponseToJSON json_adaptor;
    auto invalid_response = handler(parameters);
    BOOST_CHECK_THROW(json_adaptor(invalid_response), std::runtime_error);

    handler.send_shutdown();
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(tile_without_result_code)
{
    const constexpr char *socket_id = "ipc:///tmp/tile_without_result_code";
    ipc::Response response;
    response.set_code(200);
    response.set_code_message("message");
    std::string buffer;
    response.SerializeToString(&buffer);

    SpecialService service(buffer, socket_id);
    boost::thread threadAlpha(service);

    ipc::RequestHandler handler(socket_id);

    service::TileParameters parameters(0, 0, 12);
    adaptor::IPCResponseToJSON json_adaptor;
    BOOST_CHECK_THROW(handler(parameters), std::runtime_error);

    handler.send_shutdown();
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(annotate_empty_response)
{
    ipc::Response response;
    response.set_code(200);

    adaptor::IPCResponseToJSON json_adaptor;
    auto error = json_adaptor(response);
    auto expected = "{\"error\": \"Response message received is incomplete\"}";
    BOOST_CHECK_EQUAL(error, expected);

    response.set_code_message("message");
    auto error_msg = json_adaptor(response);
    auto expected_msg = "{\"error\": \"message\"}";
    BOOST_CHECK_EQUAL(error_msg, expected_msg);
}
