// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"

#include "ipc/request_handler.hpp"
#include "ipc/service.hpp"
#include "ipc/service_configuration.hpp"

#include "date/time.hpp"
#include "test/toolkit.hpp"

#include <algorithm>
#include <string>

#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>

#include <stdexcept>

#include <zmq.hpp>

#include "adaptor/ipc_response_to_json.hpp"

using namespace nepomuk;

struct ServiceRunner
{
    ServiceRunner(std::string socket_id) : socket_id(socket_id) {}
    std::string socket_id;

    void operator()()
    {
        ipc::ServiceConfiguration config("some_file", TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
        ipc::Service ipc_service(config);
        ipc_service.listen(socket_id);
    }
};

void sendRequest(ipc::Request const &request, zmq::socket_t &socket)
{
    std::string send_buffer;
    request.SerializeToString(&send_buffer);

    // prepare/send the message
    zmq::message_t zmq_request(send_buffer.size());
    std::copy(send_buffer.begin(), send_buffer.end(), static_cast<char *>(zmq_request.data()));
    socket.send(zmq_request);
}

void sendIncompleteRequest(ipc::Request const &request, zmq::socket_t &socket)
{
    std::string send_buffer;
    request.SerializeToString(&send_buffer);
    for (int i = 0; i < 10; ++i)
        std::next_permutation(send_buffer.begin(), send_buffer.end());

    // prepare/send the message
    zmq::message_t zmq_request(send_buffer.size());
    std::copy(send_buffer.begin(), send_buffer.end(), static_cast<char *>(zmq_request.data()));
    socket.send(zmq_request);
}

ipc::Response receiveResponse(zmq::socket_t &socket)
{
    //  wait for the reply
    zmq::message_t zmq_reply;
    socket.recv(&zmq_reply);

    ipc::Response response;
    std::string response_str;
    response_str.resize(zmq_reply.size());
    std::copy(static_cast<const char *>(zmq_reply.data()),
              static_cast<const char *>(zmq_reply.data()) + zmq_reply.size(),
              response_str.begin());
    response.ParseFromString(response_str);
    return response;
}

void send_shutdown(zmq::socket_t &socket)
{
    ipc::Request request;
    request.set_kill_switch(true);

    // prepare the message content
    std::string send_buffer;
    request.SerializeToString(&send_buffer);

    // prepare/send the message
    zmq::message_t zmq_request(send_buffer.size());
    std::copy(send_buffer.begin(), send_buffer.end(), static_cast<char *>(zmq_request.data()));
    socket.send(zmq_request);
}

BOOST_AUTO_TEST_CASE(empty_request)
{
    const constexpr char *socket_id = "ipc:///tmp/nepomuk_empty";
    ServiceRunner service(socket_id);
    boost::thread threadAlpha(service);

    ipc::Request request;

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(socket_id);

    sendRequest(request, socket);
    auto response = receiveResponse(socket);
    BOOST_CHECK(response.has_code() && response.code() == 400);
    BOOST_CHECK(response.has_code_message());
    BOOST_CHECK_EQUAL(response.code_message(), "Received and invalid or incomplete request");

    send_shutdown(socket);
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(single_coordinate)
{
    const constexpr char *socket_id = "ipc:///tmp/nepomuk_single";
    ServiceRunner service(socket_id);
    boost::thread threadAlpha(service);

    ipc::Request request;
    auto &route = *request.mutable_route();
    ipc::Coordinate origin, destination;
    origin.set_longitude(0.0);
    origin.set_latitude(0.0);
    *route.add_coordinates() = std::move(origin);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(socket_id);

    sendRequest(request, socket);
    auto response = receiveResponse(socket);
    BOOST_CHECK(response.has_code() && response.code() == 400);
    BOOST_CHECK(response.has_code_message());
    BOOST_CHECK_EQUAL(response.code_message(),
                      "Invalid parameters (Require exactly two coordinates)");

    send_shutdown(socket);
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(broken_request)
{
    const constexpr char *socket_id = "ipc:///tmp/nepomuk_broken";
    ServiceRunner service(socket_id);
    boost::thread threadAlpha(service);

    ipc::Request request;
    auto &route = *request.mutable_route();
    ipc::Coordinate origin, destination;
    origin.set_longitude(0.0);
    origin.set_latitude(0.0);
    *route.add_coordinates() = std::move(origin);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(socket_id);

    sendIncompleteRequest(request, socket);
    auto response = receiveResponse(socket);
    BOOST_CHECK(response.has_code() && response.code() == 400);
    BOOST_CHECK(response.has_code_message());
    BOOST_CHECK_EQUAL(response.code_message(), "Received and invalid or incomplete request");

    send_shutdown(socket);
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(no_departure_arrival)
{
    const constexpr char *socket_id = "ipc:///tmp/nepomuk_none";
    ServiceRunner service(socket_id);
    boost::thread threadAlpha(service);

    ipc::Request request;
    auto &route = *request.mutable_route();
    ipc::Coordinate origin, destination;
    origin.set_longitude(0.0);
    origin.set_latitude(0.0);
    *route.add_coordinates() = origin;
    *route.add_coordinates() = std::move(origin);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(socket_id);

    sendRequest(request, socket);
    auto response = receiveResponse(socket);
    BOOST_CHECK(response.has_code() && response.code() == 400);
    BOOST_CHECK(response.has_code_message());
    BOOST_CHECK_EQUAL(response.code_message(),
                      "Invalid parameters (Specify exactly one of [departure/arrival])");

    send_shutdown(socket);
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(both_departure_arrival)
{
    const constexpr char *socket_id = "ipc:///tmp/nepomuk_both";
    ServiceRunner service(socket_id);
    boost::thread threadAlpha(service);

    ipc::Request request;
    auto &route = *request.mutable_route();
    ipc::Coordinate origin, destination;
    origin.set_longitude(0.0);
    origin.set_latitude(0.0);
    *route.add_coordinates() = origin;
    *route.add_coordinates() = std::move(origin);

    route.set_utc_departure(0);
    route.set_utc_arrival(0);
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(socket_id);

    sendRequest(request, socket);
    auto response = receiveResponse(socket);
    BOOST_CHECK(response.has_code() && response.code() == 400);
    BOOST_CHECK(response.has_code_message());
    BOOST_CHECK_EQUAL(response.code_message(),
                      "Invalid parameters (Specify exactly one of [departure/arrival])");

    send_shutdown(socket);
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(walking_radii)
{
    const constexpr char *socket_id = "ipc:///tmp/nepomuk_walking_raddii";
    ServiceRunner service(socket_id);
    boost::thread threadAlpha(service);

    ipc::Request request;
    auto &route = *request.mutable_route();
    ipc::Coordinate origin, destination;
    origin.set_longitude(0.0);
    origin.set_latitude(0.0);
    *route.add_coordinates() = origin;
    *route.add_coordinates() = std::move(origin);

    route.set_utc_departure(0);
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(socket_id);

    route.add_walking_radii(0);

    sendRequest(request, socket);
    auto response = receiveResponse(socket);
    BOOST_CHECK(response.has_code() && response.code() == 400);
    BOOST_CHECK(response.has_code_message());
    BOOST_CHECK_EQUAL(response.code_message(),
                      "Invalid parameters (walking radii need to match the number of coordinates)");

    send_shutdown(socket);
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(invalid_values)
{
    const constexpr char *socket_id = "ipc:///tmp/nepomuk_invalid_route_values";
    ServiceRunner service(socket_id);
    boost::thread threadAlpha(service);

    ipc::Request request;
    auto &route = *request.mutable_route();
    ipc::Coordinate origin, destination;
    origin.set_longitude(0.0);
    origin.set_latitude(0.0);
    *route.add_coordinates() = origin;
    *route.add_coordinates() = std::move(origin);

    route.set_utc_departure(0);
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(socket_id);

    route.set_walking_speed(0);

    sendRequest(request, socket);
    auto response = receiveResponse(socket);
    BOOST_CHECK(response.has_code() && response.code() == 400);
    BOOST_CHECK(response.has_code_message());
    BOOST_CHECK_EQUAL(response.code_message(), "Invalid parameters (unspecified)");

    send_shutdown(socket);
    threadAlpha.join();
}

BOOST_AUTO_TEST_CASE(invalid_tile)
{
    const constexpr char *socket_id = "ipc:///tmp/nepomuk_invalid_tile";
    ServiceRunner service(socket_id);
    boost::thread threadAlpha(service);

    ipc::Request request;
    auto &tile = *request.mutable_tile();

    tile.set_zoom(0);
    tile.set_vertical_tile(0);
    tile.set_horizontal_tile(0);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(socket_id);

    sendRequest(request, socket);
    auto response = receiveResponse(socket);
    BOOST_CHECK(response.has_code() && response.code() == 400);
    BOOST_CHECK(response.has_code_message());
    BOOST_CHECK_EQUAL(response.code_message(), "Invalid parameters (unspecified)");

    send_shutdown(socket);
    threadAlpha.join();
}
