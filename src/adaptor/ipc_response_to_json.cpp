#include "adaptor/ipc_response_to_json.hpp"

#include <boost/assert.hpp>
#include <sstream>

#include "tool/io/string.hpp"

namespace nepomuk
{
namespace adaptor
{

namespace
{

template <typename element_type>
void wrap(
    std::ostream &os, char start, char end, element_type const &element, bool add_comma = false);

template <typename type>
void jsonify_array(std::ostream &os, type const &array, bool add_comma = false);

void tag_key(std::ostream &os, std::string const &key);
void tag(std::ostream &os,
         std::string const &key,
         std::string const &value,
         bool add_comma = false);

void jsonify(std::ostream &os, std::string const &data) { os << data; }

void jsonify(std::ostream &os, ipc::Coordinate const &coordinate)
{
    os << '[' << std::to_string(coordinate.longitude()) << ','
       << std::to_string(coordinate.latitude()) << ']';
}

void jsonify(std::ostream &os, ipc::Waypoint const &waypoint)
{
    tag_key(os, "coordinate");
    wrap(os, '[', ']', waypoint.snapped_coordinate());
}

template <typename element_type> void print_times(std::ostream &os, element_type const &element)
{
    tag(os, "departure", std::to_string(element.utc_departure()), true);
    tag(os, "arrival", std::to_string(element.utc_arrival()), true);
    tag(os, "duration", std::to_string(element.duration()), true);
}

void jsonify(std::ostream &os, ipc::Stop const &stop)
{
    if (stop.has_name())
        tag(os, "name", stop.name(), true);
    if (stop.has_utc_arrival())
        tag(os, "arrival", std::to_string(stop.utc_arrival()), true);
    if (stop.has_utc_departure())
        tag(os, "departure", std::to_string(stop.utc_departure()), true);

    tag_key(os, "location");
    jsonify(os, stop.location());
}

void jsonify(std::ostream &os, ipc::Connection const &connection)
{
    if (connection.has_headsign())
        tag(os, "headsign", connection.headsign(), true);
    if (connection.has_name())
        tag(os, "name", connection.name(), true);
    if (connection.has_name_color_text())
        tag(os, "name_color_text", connection.name_color_text(), true);
    if (connection.has_name_color_background())
        tag(os, "name_color_background", connection.name_color_background(), true);

    tag(os, "departure", std::to_string(connection.utc_departure()), true);
    tag(os, "arrival", std::to_string(connection.utc_arrival()));
}

void jsonify(std::ostream &os, ipc::TransitSegment const &transit)
{
    print_times(os, transit);
    tag(os, "geometry", transit.polyline(), true);
    tag_key(os, "stops");
    jsonify_array(os, transit.stops(), true);
    tag_key(os, "connections");
    jsonify_array(os, transit.connections());
}

void jsonify(std::ostream &os, ipc::WalkingSegment const &walk)
{
    print_times(os, walk);
    tag(os, "geometry", walk.polyline(), true);
    tag(os, "distance", std::to_string(walk.distance()));
}

void jsonify(std::ostream &os, ipc::TransferSegment const &transfer)
{
    print_times(os, transfer);
    tag(os, "geometry", transfer.polyline(), true);
    tag_key(os, "origin");
    wrap(os, '{', '}', transfer.origin(), true);
    tag_key(os, "destination");
    wrap(os, '{', '}', transfer.destination());
}

void jsonify(std::ostream &os, ipc::Segment const &segment)
{
    if (segment.has_transit())
        jsonify(os, segment.transit());
    else if (segment.has_walk())
        jsonify(os, segment.walk());
    else
    {
        BOOST_ASSERT(segment.has_transfer());
        jsonify(os, segment.transfer());
    }
}

void jsonify(std::ostream &os, ipc::Leg const &leg)
{
    print_times(os, leg);
    tag_key(os, "segments");
    jsonify_array(os, leg.segments());
}

void jsonify(std::ostream &os, ipc::Route const &route)
{
    print_times(os, route);
    tag_key(os, "legs");
    jsonify_array(os, route.legs());
}

void jsonify(std::ostream &os, ipc::RouteResponse const &response)
{
    tag(os, "code", "Ok", true);
    tag_key(os, "routes");
    jsonify_array(os, response.routes(), true);
    tag_key(os, "waypoints");
    jsonify_array(os, response.waypoints());
}

template <typename element_type>
void wrap(std::ostream &os, char start, char end, element_type const &element, bool add_comma)
{
    os << start;
    jsonify(os, element);
    os << end;
    if (add_comma)
        os << ",";
}

template <typename type> void jsonify_array(std::ostream &os, type const &array, bool add_comma)
{
    os << '[';
    for (std::size_t index = 0; index < array.size(); ++index)
        wrap(os, '{', '}', array[index], index + 1 < array.size());
    os << ']';
    if (add_comma)
        os << ',';
}

void tag_key(std::ostream &os, std::string const &key)
{
    wrap(os, '\"', '\"', key);
    os << ':';
}

void tag(std::ostream &os, std::string const &key, std::string const &value, bool add_comma)
{
    tag_key(os, key);
    wrap(os, '\"', '\"', value);
    if (add_comma)
        os << ',';
}

} // namespace

std::string IPCResponseToJSON::operator()(ipc::RouteResponse const &response)
{
    std::ostringstream os;
    wrap(os, '{', '}', response);
    return os.str();
}

std::string IPCResponseToJSON::operator()(ipc::TileResponse const &response)
{
    if (!response.has_result())
        throw std::runtime_error("TileResponse does not contain any vector_tile");

    return response.result();
}

std::string IPCResponseToJSON::operator()(ipc::Response const &response)
{
    if (response.has_tile())
        return (*this)(response.tile());
    else if (response.has_route())
        return (*this)(response.route());
    else if (response.has_code_message())
        return "{\"error\": \"" + response.code_message() + "\"}";
    else
        return "{\"error\": \"Response message received is incomplete\"}";
}

} // namespace adaptor
} // namespace nepomuk
