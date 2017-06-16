#include "annotation/pbf.hpp"
#include "annotation/geometry.hpp"

#include "navigation/connection.hpp"
#include "navigation/leg.hpp"
#include "navigation/route.hpp"
#include "navigation/segment.hpp"
#include "navigation/stop.hpp"

#include "geometric/coordinate.hpp"
#include "geometric/polyline.hpp"

#include "date/time.hpp" // for UTCTimestamp

#include "tool/io/string.hpp"

#include <boost/assert.hpp>

#include <algorithm> // transform

namespace nepomuk
{
namespace annotation
{

PBF::PBF(Geometry const &geometry) : geometry(geometry) {}

void PBF::operator()(ipc::RouteResponse &result, std::vector<navigation::Route> const &routes) const
{
    add_waypoints(result, routes.front());
    for (auto const &route : routes)
        add(*result.add_routes(), route);
}

void PBF::add_waypoints(ipc::RouteResponse &target, navigation::Route const &route) const
{
    auto const legs = route.legs();
    BOOST_ASSERT(!legs.empty());

    auto as_ipc_coordinate = [](geometric::WGS84Coordinate coordinate) {
        ipc::Coordinate result;
        result.set_longitude(geometric::doubleFromLatLon(coordinate.longitude));
        result.set_latitude(geometric::doubleFromLatLon(coordinate.latitude));
        return result;
    };

    std::for_each(route.legs().begin(), route.legs().end(), [&](auto const &leg) {
        auto &waypoint = *target.add_waypoints();
        auto const &segment = leg.segments().front();
        BOOST_ASSERT(!segment.is_transfer());
        if (segment.is_transit())
        {
            auto const &transit = segment.as_transit();
            *waypoint.mutable_snapped_coordinate() =
                as_ipc_coordinate(geometry.get(transit.stops().front().id()));
        }
        else
        {
            BOOST_ASSERT(segment.is_walk());
            auto const &walk = segment.as_walk();
            *waypoint.mutable_snapped_coordinate() = as_ipc_coordinate(walk.origin());
        }
    });

    auto const &segment = route.legs().back().segments().back();
    auto &waypoint = *target.add_waypoints();
    if (segment.is_transit())
    {
        auto const &transit = segment.as_transit();
        *waypoint.mutable_snapped_coordinate() =
            as_ipc_coordinate(geometry.get(transit.stops().back().id()));
    }
    else
    {
        BOOST_ASSERT(segment.is_walk());
        auto const &walk = segment.as_walk();
        *waypoint.mutable_snapped_coordinate() = as_ipc_coordinate(walk.destination());
    }
}

void PBF::add(ipc::Route &target, navigation::Route const &route) const
{
    for (auto const &leg : route.legs())
        add(*target.add_legs(), leg);

    target.set_utc_departure(route.departure().seconds_since_epoch);
    target.set_utc_arrival(route.arrival().seconds_since_epoch);
    target.set_duration(route.duration());
}

void PBF::add(ipc::Leg &target, navigation::Leg const &leg) const
{
    for (auto const &segment : leg.segments())
        add(*target.add_segments(), segment);

    target.set_utc_departure(leg.departure().seconds_since_epoch);
    target.set_utc_arrival(leg.arrival().seconds_since_epoch);
    target.set_duration(leg.duration());
}

void PBF::add(ipc::Segment &target, navigation::Segment const &segment) const
{
    if (segment.is_transit())
        add(*target.mutable_transit(), segment.as_transit());
    else if (segment.is_transfer())
        add(*target.mutable_transfer(), segment.as_transfer());
    else
    {
        BOOST_ASSERT(segment.is_walk());
        add(*target.mutable_walk(), segment.as_walk());
    }
}

void PBF::add(ipc::TransitSegment &target, navigation::segment::Transit const &transit) const
{
    target.set_utc_departure(transit.departure().seconds_since_epoch);
    target.set_utc_arrival(transit.arrival().seconds_since_epoch);
    target.set_duration(transit.duration());

    // Get coordinates for items stored in geometry
    auto const range = geometry.get(transit.connections().front().line(),
                                    transit.stops().front().id(),
                                    transit.stops().back().id());
    std::vector<geometric::WGS84Coordinate> coordinates(range.begin(), range.end());

    // or take the stops if we don't have geometry
    if (range.empty())
    {
        std::transform(transit.stops().begin(),
                       transit.stops().end(),
                       std::back_inserter(coordinates),
                       [&](auto const &stop) { return geometry.get(stop.id()); });
    }
    target.set_polyline(tool::io::to_escaped(geometric::Polyline::encode(100000, coordinates)));

    for (auto const &stop : transit.stops())
        add(*target.add_stops(), stop);

    for (auto const &connection : transit.connections())
        add(*target.add_connections(), connection);
}

void PBF::add(ipc::TransferSegment &target, navigation::segment::Transfer const &transfer) const
{
    target.set_utc_departure(transfer.departure().seconds_since_epoch);
    target.set_utc_arrival(transfer.arrival().seconds_since_epoch);
    target.set_duration(transfer.duration());

    std::vector<geometric::WGS84Coordinate> coordinates = {geometry.get(transfer.origin()),
                                                           geometry.get(transfer.destination())};

    target.set_polyline(tool::io::to_escaped(geometric::Polyline::encode(100000, coordinates)));

    auto &origin = *target.mutable_origin();
    origin.set_name("It's coming, some day");
    add(*origin.mutable_location(), coordinates[0]);
    auto &destination = *target.mutable_destination();
    destination.set_name("It's coming, some day");
    add(*destination.mutable_location(), coordinates[1]);
}

void PBF::add(ipc::WalkingSegment &target, navigation::segment::Walk const &walk) const
{
    target.set_utc_departure(walk.departure().seconds_since_epoch);
    target.set_utc_arrival(walk.arrival().seconds_since_epoch);
    target.set_duration(walk.duration());
    target.set_distance(walk.distance());
    std::vector<geometric::WGS84Coordinate> coordinates = {walk.origin(), walk.destination()};
    target.set_polyline(tool::io::to_escaped(geometric::Polyline::encode(100000, coordinates)));
}

void PBF::add(ipc::Stop &target, navigation::Stop const &stop) const
{
    target.set_name("Names will show up here");
    target.set_utc_departure(stop.departure().seconds_since_epoch);
    target.set_utc_arrival(stop.arrival().seconds_since_epoch);
    add(*target.mutable_location(), geometry.get(stop.id()));
}

void PBF::add(ipc::Coordinate &target, geometric::WGS84Coordinate const coordinate) const
{
    target.set_longitude(geometric::doubleFromLatLon(coordinate.longitude));
    target.set_latitude(geometric::doubleFromLatLon(coordinate.latitude));
}

void PBF::add(ipc::Connection &target, navigation::Connection const &connection) const
{
    target.set_headsign("Headsigns are so important");
    target.set_name("Linenames even more important");
    target.set_utc_departure(connection.departure().seconds_since_epoch);
    target.set_utc_arrival(connection.departure().seconds_since_epoch);
    target.set_name_color_text("#000000");
    target.set_name_color_background("#FFFFFF");
}

} // namespace annotation
} // namespace nepomuk
