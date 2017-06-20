#include "ipc/service.hpp"
#include "ipc/service_configuration.hpp"

#include "ipc/request.pb.h"
#include "ipc/response.pb.h"
#include "ipc/route_request.pb.h"
#include "ipc/tile_request.pb.h"

#include "geometric/coordinate.hpp"

#include "date/time.hpp"

#include <algorithm>
#include <boost/assert.hpp>
#include <boost/optional.hpp>
#include <zmq.hpp>

namespace nepomuk
{
namespace ipc
{

namespace
{

std::string invalid_parameters(std::string message = "unspecified")
{
    Response pbf_response;
    pbf_response.set_code(400);
    pbf_response.set_code_message("Invalid parameters (" + message + ")");

    std::string buffer;
    pbf_response.SerializeToString(&buffer);
    return buffer;
}

// returns a string containing a serialised PBF RouteResponse
std::string handle_request(RouteRequest const &request,
                           service::Route const &service,
                           annotation::PBF const &pbf_annotator)
{
    // need to have exactly two coordinates
    if (request.coordinates_size() != 2)
        return invalid_parameters("Require exactly two coordinates");

    // either departure or arrival
    if ((!request.has_utc_arrival() && !request.has_utc_departure()) ||
        (request.has_utc_arrival() && request.has_utc_departure()))
        return invalid_parameters("Specify exactly one of [departure/arrival]");

    if (request.walking_radii_size() > 0 &&
        request.walking_radii_size() != request.coordinates_size())
        return invalid_parameters("walking radii need to match the number of coordinates");

    auto const departure = [&]() -> boost::optional<date::UTCTimestamp> {
        if (request.has_utc_departure())
            return date::UTCTimestamp{request.utc_departure()};
        else
            return boost::none;
    }();

    auto const arrival = [&]() -> boost::optional<date::UTCTimestamp> {
        if (request.has_utc_arrival())
            return date::UTCTimestamp{request.utc_arrival()};
        else
            return boost::none;
    }();

    auto to_wgs_coordinate = [](auto const &pbf_coordinate) -> geometric::WGS84Coordinate {
        return {
            geometric::makeLatLonFromDouble<geometric::FixedLongitude>(pbf_coordinate.longitude()),
            geometric::makeLatLonFromDouble<geometric::FixedLatitude>(pbf_coordinate.latitude())};
    };

    service::RouteParameters parameters(
        to_wgs_coordinate(request.coordinates()[0]),
        to_wgs_coordinate(request.coordinates()[1]),
        departure,
        arrival,
        request.walking_radii_size() ? request.walking_radii()[0] : 1500.0,
        request.has_walking_speed() ? request.walking_speed() : 1.0,
        request.has_transfer_scale() ? request.transfer_scale() : 1.0);

    if (!parameters.valid())
        return invalid_parameters();

    // run the request
    auto const routes = service(parameters);

    Response pbf_response;
    if (!routes.empty())
    {
        // convert to pbf
        pbf_response.set_code(200);
        pbf_annotator(*pbf_response.mutable_route(), routes);
    }
    else
    {
        pbf_response.set_code(200);
        pbf_response.set_code_message("No Route");
    }

    // and convert to buffer
    std::string buffer;
    pbf_response.SerializeToString(&buffer);

    return buffer;
}

// returns a string containing a serialised PBF TileResponse
std::string handle_request(TileRequest const &request, service::Tile const &service)
{
    service::TileParameters parameters(
        request.horizontal_tile(), request.vertical_tile(), request.zoom());

    if (!parameters.valid())
        return invalid_parameters();

    Response pbf_response;
    auto &result_tile = *pbf_response.mutable_tile()->mutable_result();
    result_tile = service(parameters);

    std::string buffer;
    pbf_response.SerializeToString(&buffer);

    return buffer;
}

} // namespace

Service::Service(ServiceConfiguration const &configuration)
    : master(configuration.dataset()), route(master), tile(master),
      pbf_annotator(master.pbf_annotation())
{
}

void Service::listen(std::string const &socket_str)
{
    const int constexpr num_threads = 1;
    zmq::context_t context(num_threads);
    zmq::socket_t socket(context, ZMQ_REP);

    // listen on the requested port
    socket.bind(socket_str);

    auto const invalid_buffer = []() {
        Response pbf_response;
        pbf_response.set_code(400);
        pbf_response.set_code_message("Received and invalid or incomplete request");

        std::string buffer;
        pbf_response.SerializeToString(&buffer);
        return buffer;
    }();

    auto const healthy = []() {
        Response pbf_response;
        pbf_response.set_code(200);
        pbf_response.set_code_message("I am awake!");

        std::string buffer;
        pbf_response.SerializeToString(&buffer);
        return buffer;

    }();

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

        Request pbf_request;

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
                else
                {
                    send_buffered_response(healthy);
                }
            }
            else
            {

                // hand of the request to the services
                auto const buffer = [&]() {
                    if (pbf_request.has_route())
                    {
                        return handle_request(pbf_request.route(), route, pbf_annotator);
                    }
                    else if (pbf_request.has_tile())
                    {
                        return handle_request(pbf_request.tile(), tile);
                    }
                    else
                    {
                        return invalid_buffer;
                    }
                }();

                send_buffered_response(buffer);
            }
        }
        // received an invalid or incomplete message
        else
        {
            send_buffered_response(invalid_buffer);
        }
    }
}

} // namespace ipc
} // namespace nepomuk
