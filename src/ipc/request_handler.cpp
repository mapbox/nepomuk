#include "ipc/request_handler.hpp"
#include "ipc/context.hpp"

#include <algorithm>

namespace nepomuk
{
namespace ipc
{

RequestHandler::RequestHandler(std::string const &socket_id)
    : context(get_context()), socket(context, ZMQ_REQ)
{
    // open up a connection, no request yet
    socket.connect(socket_id);
}

Response RequestHandler::do_request(Request const &request)
{
    // prepare the message content
    std::string send_buffer;
    request.SerializeToString(&send_buffer);

    // prepare/send the message
    zmq::message_t zmq_request(send_buffer.size());
    std::copy(send_buffer.begin(), send_buffer.end(), static_cast<char *>(zmq_request.data()));
    socket.send(zmq_request);

    //  wait for the reply
    zmq::message_t zmq_reply;
    socket.recv(&zmq_reply);

    Response response;
    std::string response_str;
    response_str.resize(zmq_reply.size());
    std::copy(static_cast<const char *>(zmq_reply.data()),
              static_cast<const char *>(zmq_reply.data()) + zmq_reply.size(),
              response_str.begin());
    response.ParseFromString(response_str);
    return response;
}

Response RequestHandler::operator()(service::TileParameters parameters)
{
    auto wrap = [](auto const &parameters) {
        Request request;
        auto &tile = *request.mutable_tile();
        tile.set_zoom(parameters.zoom_level());
        tile.set_horizontal_tile(parameters.horizontal_id());
        tile.set_vertical_tile(parameters.vertical_id());
        return request;
    };

    auto response = do_request(wrap(parameters));
    if (!response.has_tile())
    {
        if (response.has_code_message())
            throw std::runtime_error(response.code_message());
        else
            throw std::runtime_error("unknown");
    }
    return response;
}

Response RequestHandler::operator()(service::RouteParameters parameters)
{
    auto wrap = [](auto const &parameters) {
        Request request;
        auto &route = *request.mutable_route();

        ipc::Coordinate origin, destination;
        origin.set_longitude(geometric::doubleFromLatLon(parameters.origin().longitude));
        origin.set_latitude(geometric::doubleFromLatLon(parameters.origin().latitude));
        *route.add_coordinates() = std::move(origin);

        destination.set_longitude(geometric::doubleFromLatLon(parameters.destination().longitude));
        destination.set_latitude(geometric::doubleFromLatLon(parameters.destination().latitude));
        *route.add_coordinates() = std::move(destination);

        if (parameters.departure())
            route.set_utc_departure(parameters.departure()->seconds_since_epoch);
        if (parameters.arrival())
            route.set_utc_arrival(parameters.arrival()->seconds_since_epoch);

        route.add_walking_radii(parameters.walking_radius());
        route.add_walking_radii(parameters.walking_radius());

        route.set_transfer_scale(parameters.transfer_scale());
        route.set_walking_speed(parameters.walking_speed());

        return request;
    };

    auto response = do_request(wrap(parameters));
    if (!response.has_route())
    {
        if (response.has_code_message())
            throw std::runtime_error(response.code_message());
        else
            throw std::runtime_error("unknown");
    }

    return response;
}

void RequestHandler::send_shutdown()
{
    Request request;
    request.set_kill_switch(true);

    // prepare the message content
    std::string send_buffer;
    request.SerializeToString(&send_buffer);

    // prepare/send the message
    zmq::message_t zmq_request(send_buffer.size());
    std::copy(send_buffer.begin(), send_buffer.end(), static_cast<char *>(zmq_request.data()));
    socket.send(zmq_request);
}

Response RequestHandler::check_alive()
{
    Request request;
    request.set_kill_switch(false);

    return do_request(request);
}

} // namespace ipc
} // namespace nepomuk
