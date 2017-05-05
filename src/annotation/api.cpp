#include "annotation/api.hpp"
#include "annotation/geometry.hpp"

#include "navigation/connection.hpp"
#include "navigation/leg.hpp"
#include "navigation/route.hpp"
#include "navigation/segment.hpp"
#include "navigation/stop.hpp"

#include "geometric/coordinate.hpp"
#include "geometric/polyline.hpp"

#include "date/time.hpp" // for UTCTimestamp

#include <algorithm> // for transform
#include <boost/assert.hpp>
#include <boost/range/iterator_range_core.hpp> // for iterator_range
#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>      // for allocator, char_traits
#include <type_traits> // for enable_if<>::type
#include <utility>     // for make_pair, pair

namespace transit
{
namespace annotation
{

namespace
{
struct BraceGuard
{
    BraceGuard(std::ostream &os) : os(os) { os << "{"; }

    ~BraceGuard() { os << "}"; }
    std::ostream &os;
};

void quote(std::ostream &os, std::string const &value) { os << "\"" << value << "\""; }
void tag(std::ostream &os, std::string const &value)
{
    quote(os, value);
    os << ":";
}

} // namespace

API::API(Geometry const &geometry) : geometry(geometry) {}

std::string API::operator()(std::vector<navigation::Route> const &routes) const
{
    std::ostringstream os;
    // do not use guar here, since we return result before closing the scope
    os << "{";
    tag(os, "code");
    quote(os, "Ok");
    os << ",";

    tag(os, "waypoints");
    jsonify_waypoints(os, routes.front());
    os << ",";

    tag(os, "routes");
    chain_jsonify(os, routes.begin(), routes.end());
    os << "}";
    return os.str();
}

void API::jsonify_waypoints(std::ostream &os, navigation::Route const &route) const
{
    auto const legs = route.legs();
    BOOST_ASSERT(!legs.empty());
    std::vector<geometric::WGS84Coordinate> waypoints(legs.size() + 1);

    auto as_coordinate_pair = [&](auto const &segment) {
        // a transfer can only happen in between destinations
        BOOST_ASSERT(!segment.is_transfer());
        if (segment.is_transit())
        {
            auto const &transit = segment.as_transit();
            return std::make_pair(geometry.get(transit.stops().front().id()),
                                  geometry.get(transit.stops().back().id()));
        }
        else
        {
            BOOST_ASSERT(segment.is_walk());
            auto const &walk = segment.as_walk();
            return std::make_pair(walk.origin(), walk.destination());
        }
    };

    std::transform(legs.begin(), legs.end(), waypoints.begin(), [&](auto const &leg) {
        return as_coordinate_pair(leg.segments().front()).first;
    });

    waypoints.back() = as_coordinate_pair(legs.back().segments().back()).second;

    chain_jsonify(os, waypoints.begin(), waypoints.end());
}

void API::jsonify(std::ostream &os, navigation::Route const &route) const
{
    BraceGuard brace_guard(os);

    tag(os, "depart");
    os << route.departure().seconds_since_epoch << ",";

    tag(os, "arrive");
    os << route.arrival().seconds_since_epoch << ",";

    tag(os, "duration");
    os << route.duration() << ",";

    tag(os, "summary");
    quote(os, "most beautiful route");
    os << ",";
    tag(os, "legs");
    auto const legs = route.legs();
    chain_jsonify(os, legs.begin(), legs.end());
}

void API::jsonify(std::ostream &os, navigation::Leg const &leg) const
{
    BraceGuard brace_guard(os);
    tag(os, "depart");
    os << leg.departure().seconds_since_epoch << ",";
    tag(os, "arrive");
    os << leg.arrival().seconds_since_epoch << ",";
    tag(os, "duration");
    os << leg.duration() << ",";
    tag(os, "segments");
    auto const segments = leg.segments();
    chain_jsonify(os, segments.begin(), segments.end());
}

void API::jsonify(std::ostream &os, navigation::Segment const &segment) const
{
    BraceGuard brace_guard(os);
    if (segment.is_transit())
        jsonify(os, segment.as_transit());
    else if (segment.is_transfer())
        jsonify(os, segment.as_transfer());
    else
    {
        BOOST_ASSERT(segment.is_walk());
        jsonify(os, segment.as_walk());
    }
}

void API::jsonify(std::ostream &os, navigation::segment::Transit const &transit) const
{
    // does not need guard, already guarded
    tag(os, "mode");
    quote(os, "transit");
    os << ",";
    tag(os, "depart");
    os << transit.departure().seconds_since_epoch << ",";
    tag(os, "arrive");
    os << transit.arrival().seconds_since_epoch << ",";
    tag(os, "duration");
    os << transit.duration() << ",";
    tag(os, "geometry");
    auto const range = geometry.get(transit.connections().front().line(),
                                    transit.stops().front().id(),
                                    transit.stops().back().id());
    std::vector<geometric::WGS84Coordinate> coordinates(range.begin(), range.end());
    if (range.empty())
    {
        std::transform(transit.stops().begin(),
                       transit.stops().end(),
                       std::back_inserter(coordinates),
                       [&](auto const &stop) { return geometry.get(stop.id()); });
    }
    jsonify(os, coordinates);
    os << ",";
    tag(os, "stops");
    auto const stops = transit.stops();
    chain_jsonify(os, stops.begin(), stops.end());
    os << ",";
    tag(os, "connections");
    auto const connections = transit.connections();
    chain_jsonify(os, connections.begin(), connections.end());
}

void API::jsonify(std::ostream &os, navigation::segment::Transfer const &transfer) const
{
    // does not need guard, already guarded
    tag(os, "mode");
    quote(os, "transfer");
    os << ",";

    tag(os, "depart");
    os << transfer.departure().seconds_since_epoch << ",";

    tag(os, "arrive");
    os << transfer.arrival().seconds_since_epoch << ",";

    tag(os, "stopname");
    quote(os, "Stopname will be here, believe me.");
    os << ",";

    tag(os, "geometry");
    std::vector<geometric::WGS84Coordinate> coordinates = {geometry.get(transfer.origin()),
                                                           geometry.get(transfer.destination())};
    jsonify(os, coordinates);
    os << ",";

    tag(os, "duration");
    os << transfer.duration();
}

void API::jsonify(std::ostream &os, navigation::segment::Walk const &walk) const
{
    // does not need guard, already guarded
    tag(os, "mode");
    quote(os, "walking");
    os << ",";

    tag(os, "depart");
    os << walk.departure().seconds_since_epoch << ",";

    tag(os, "arrive");
    os << walk.arrival().seconds_since_epoch << ",";

    tag(os, "duration");
    os << walk.duration() << ",";

    tag(os, "distance");
    os << walk.distance() << ",";

    tag(os, "geometry");
    std::vector<geometric::WGS84Coordinate> coordinates = {walk.origin(), walk.destination()};
    jsonify(os, coordinates);
}

void API::jsonify(std::ostream &os, navigation::Stop const &stop) const
{
    BraceGuard brace_guard(os);
    tag(os, "name");
    quote(os, "NAME HERE");
    os << ",";

    tag(os, "location");
    jsonify(os, geometric::WGS84Coordinate());
    os << ",";

    tag(os, "arrive");
    os << stop.arrival().seconds_since_epoch << ",";

    tag(os, "depart");
    os << stop.departure().seconds_since_epoch;
}

void API::jsonify(std::ostream &os, navigation::Connection const &connection) const
{
    BraceGuard brace_guard(os);

    tag(os, "depart");
    os << connection.departure().seconds_since_epoch << ",";

    tag(os, "arrive");
    os << connection.arrival().seconds_since_epoch << ",";

    tag(os, "duration");
    os << connection.duration() << ",";

    tag(os, "name");
    quote(os, "CONNECTION NAME HERE");
    os << ",";

    tag(os, "headsign");
    quote(os, "HEADSIGN HERE");
}

void API::jsonify(std::ostream &os, geometric::WGS84Coordinate const coordinate) const
{
    os << "[" << std::setprecision(12) << doubleFromLatLon(coordinate.longitude) << ","
       << doubleFromLatLon(coordinate.latitude) << "]";
}

void API::jsonify(std::ostream &os,
                  std::vector<geometric::WGS84Coordinate> const &coordinates) const
{
    quote(os, geometric::Polyline::encode(100000, coordinates));
}

} // namespace annotation
} // namespace transit
