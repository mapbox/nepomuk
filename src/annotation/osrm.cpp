#include "annotation/osrm.hpp"
#include "geometric/coordinate.hpp"
#include "geometric/polyline.hpp"

#include <numeric>
#include <sstream>

// TODO remove
#include <cctype>
#include <iomanip>
#include <iostream>

namespace transit
{
namespace annotation
{

namespace
{
auto const constexpr POLYLINE_PRECISION = 100000;

void write_escaped(std::ostringstream &oss, std::string const &value)
{
    auto const requires_escape = [](char const letter) {
        auto const is_quote = letter == '\"';
        auto const is_slash = letter == '\\';
        auto const is_backslash = letter == '/';
        return is_quote || is_slash || is_backslash;
    };
    auto const write_escaped_char = [&](char const letter) {
        if (iscntrl(letter) || requires_escape(letter))
            oss << "\\" << letter;
        else
            oss << letter;
    };
    std::for_each(value.begin(), value.end(), write_escaped_char);
}
} // namespace

void OSRM::write_maneuver(std::ostringstream &oss, StopID const stop, std::string const &type) const
{
    oss << "\"maneuver\":{\"location\": [";
    auto const location = geometry.get(stop);
    oss << std::setprecision(12) << doubleFromLatLon(location.longitude) << ","
        << doubleFromLatLon(location.latitude);
    oss << "],\"type\": \"" << type << "\",\"modifier\": \"straight\"}";
}

void OSRM::write_intersections(std::ostringstream &oss, StopID const stop) const
{
    oss << "\"intersections\": [";
    oss << "{\"location\": [";
    auto const location = geometry.get(stop);
    oss << std::setprecision(12) << doubleFromLatLon(location.longitude) << ","
        << doubleFromLatLon(location.latitude);
    oss << "]}";
    oss << "],";
}

void OSRM::write_segments(std::ostringstream &oss, navigation::Leg const &leg) const
{
    bool print = false;

    for (auto const &segment : leg.segments())
    {
        if (print)
            oss << ",";
        print = true;

        if (segment.is_transit())
        {
            write_segment(oss, segment.as_transit());
        }
        else if (segment.is_transfer())
        {
            write_segment(oss, segment.as_transfer());
        }
        else
        {
            write_segment(oss, segment.as_walk());
        }
    }
}

void OSRM::write_segment(std::ostringstream &oss, navigation::segment::Transit const &transit) const
{
    auto const stops = transit.stops();
    // print departure
    {
        oss << "{";
        oss << "\"geometry\": \"";
        auto crange = geometry.get(
            transit.connections().front().line(), stops.front().id(), (stops.begin() + 1)->id());
        std::vector<geometric::WGS84Coordinate> coordinates(crange.begin(), crange.end());
        if (coordinates.empty())
            coordinates.push_back(geometry.get(stops.front().id()));
        write_escaped(oss, geometric::Polyline::encode(POLYLINE_PRECISION, coordinates));
        oss << "\",";
        write_intersections(oss, stops.front().id());
        oss << "\"mode\": \"transit\",";
        oss << "\"name\": \""
            << dictionary.get_string(stop_info.get_info(stops.front().id()).name_id) << "\",";
        oss << "\"distance\": 1,";
        oss << "\"duration\": " << ((stops.begin() + 1)->arrival() - stops.front().departure())
            << ",";
        // write_maneuver(oss, range.front().stop_id, "board");
        write_maneuver(oss, stops.front().id(), "depart");
        oss << "}";
    }

    if (stops.size() > 3)
    {
        for (auto itr = stops.begin() + 1; itr + 1 != stops.end(); ++itr)
        {
            // only print at intermediate parts
            oss << ",";
            oss << "{";
            oss << "\"geometry\": \"";
            auto crange =
                geometry.get(transit.connections().front().line(), itr->id(), (itr + 1)->id());
            std::vector<geometric::WGS84Coordinate> coordinates(crange.begin(), crange.end());
            if (crange.empty())
            {
                coordinates.push_back(geometry.get(itr->id()));
                coordinates.push_back(geometry.get((itr + 1)->id()));
            }
            write_escaped(oss, geometric::Polyline::encode(POLYLINE_PRECISION, coordinates));
            oss << "\",";
            write_intersections(oss, stops.front().id());
            oss << "\"mode\": \"transit\",";
            oss << "\"name\": \"" << dictionary.get_string(stop_info.get_info(itr->id()).name_id)
                << "\",";
            oss << "\"distance\": 1,";
            oss << "\"duration\": " << ((itr + 1)->arrival() - itr->arrival()) << ",";
            write_maneuver(oss, itr->id(), "stay in vehicle");
            oss << "}";
        }
    }
    {
        // only print at intermediate parts
        oss << ",{";
        oss << "\"geometry\": \"";
        std::vector<geometric::WGS84Coordinate> coordinates;
        coordinates.push_back(geometry.get(stops.back().id()));
        write_escaped(oss, geometric::Polyline::encode(POLYLINE_PRECISION, coordinates));
        oss << "\",";
        write_intersections(oss, stops.back().id());
        oss << "\"mode\": \"transit\",";
        oss << "\"name\": \""
            << dictionary.get_string(stop_info.get_info(stops.back().id()).name_id) << "\",";
        oss << "\"distance\": 1,";
        oss << "\"duration\": 0,";
        // write_maneuver(oss, range.back().stop_id, "deboard");
        write_maneuver(oss, stops.back().id(), "arrive");
        oss << "}";
    }
}

void OSRM::write_segment(std::ostringstream &oss,
                         navigation::segment::Transfer const &transfer) const
{
    oss << "{";
    oss << "\"geometry\": \"";
    std::vector<geometric::WGS84Coordinate> coordinates;
    coordinates.push_back(geometry.get(transfer.origin()));
    coordinates.push_back(geometry.get(transfer.destination()));
    write_escaped(oss, geometric::Polyline::encode(POLYLINE_PRECISION, coordinates));
    oss << "\",";
    write_intersections(oss, transfer.origin());
    oss << "\"mode\": \"walk\",";
    oss << "\"name\": \""
        << dictionary.get_string(stop_info.get_info(transfer.destination()).name_id) << "\",";
    oss << "\"distance\": " << geometric::distance(geometry.get(transfer.origin()),
                                                   geometry.get(transfer.destination()))
        << ",";
    oss << "\"duration\": " << transfer.duration() << ",";
    write_maneuver(oss, transfer.origin(), "transfer");
    oss << "}";
}

void OSRM::write_segment(std::ostringstream &oss, navigation::segment::Walk const &walk) const
{
    oss << "{";
    oss << "\"geometry\": \"";
    std::vector<geometric::WGS84Coordinate> coordinates;
    coordinates.push_back(walk.origin());
    coordinates.push_back(walk.destination());
    write_escaped(oss, geometric::Polyline::encode(POLYLINE_PRECISION, coordinates));
    std::cout << "Origin: " << walk.origin() << " Destination: " << walk.destination() << std::endl;
    oss << "\",";
    oss << "\"mode\": \"walk\",";
    oss << "\"distance\": " << geometric::distance(walk.origin(), walk.destination()) << ",";
    oss << "\"duration\": " << walk.duration() << ",";
    oss << "\"maneuver\":{\"location\": [";
    auto const location = walk.origin();
    oss << std::setprecision(12) << doubleFromLatLon(location.longitude) << ","
        << doubleFromLatLon(location.latitude);
    oss << "],\"type\": \"walk\",\"modifier\": \"straight\"}";
    oss << "}";
}

void OSRM::write_leg(std::ostringstream &oss, navigation::Leg const &leg) const
{
    oss << "{";
    oss << "\"distance\": " << std::distance(leg.segments().begin(), leg.segments().end()) << ",";
    oss << "\"duration\": " << leg.duration() << ",";
    oss << "\"weight\": " << leg.duration() << ",";
    oss << "\"summary\": "
        << "\"LINE_NAME_HERE\","; // TODO
    oss << "\"steps\":[";
    write_segments(oss, leg);
    oss << "]";
    oss << "}";
}

void OSRM::write_legs(std::ostringstream &oss, navigation::Route const &route) const
{
    bool start_with_comma = false;
    // Write out information about a route
    for (auto const &leg : route.legs())
    {
        if (start_with_comma)
            oss << ",";
        write_leg(oss, leg);
        start_with_comma = true;
    }
}

void OSRM::write_coordinates(std::ostringstream &oss, navigation::Route const &route) const
{
    bool start_with_comma = false;
    std::vector<geometric::WGS84Coordinate> coordinates;
    // Write out information about a route
    for (auto const &leg : route.legs())
    {
        for (auto const &segment : leg.segments())
        {
            if (segment.is_transit())
            {
                auto const &transit = segment.as_transit();
                auto stops = transit.stops();
                auto crange = geometry.get(
                    transit.connections().front().line(), stops.front().id(), stops.back().id());
                if (crange.empty())
                {
                    std::transform(stops.begin(),
                                   stops.end(),
                                   std::back_inserter(coordinates),
                                   [&](auto const &stop) { return geometry.get(stop.id()); });
                }
                else
                {
                    coordinates.insert(coordinates.end(), crange.begin(), crange.end());
                }
            }
            else if (segment.is_transfer())
            {
                auto const &transfer = segment.as_transfer();
                coordinates.push_back(geometry.get(transfer.origin()));
                coordinates.push_back(geometry.get(transfer.destination()));
            }
            else
            {
                auto const &walk = segment.as_walk();
                coordinates.push_back(walk.origin());
                coordinates.push_back(walk.destination());
            }
        }
    }
    write_escaped(oss, geometric::Polyline::encode(100000, coordinates));
}

void OSRM::write_waypoint(std::ostringstream &oss, StopID const stop) const
{
    oss << "{\"distance\": 0, \"name\": \"";
    oss << dictionary.get_string(stop_info.get_info(stop).name_id);
    oss << "\", \"location\" : [";
    auto const location = geometry.get(stop);
    oss << std::setprecision(12) << doubleFromLatLon(location.longitude) << ","
        << doubleFromLatLon(location.latitude);
    oss << "]}";
}

void OSRM::write_waypoint(std::ostringstream &oss, geometric::WGS84Coordinate const location) const
{
    oss << "{\"distance\": 0, \"name\": \"UNKNOWN\", \"location\" : [";
    oss << std::setprecision(12) << doubleFromLatLon(location.longitude) << ","
        << doubleFromLatLon(location.latitude);
    oss << "]}";
}

void OSRM::write_waypoints(std::ostringstream &oss, navigation::Route const &route) const
{

    oss << "\"waypoints\":[";

    auto const legs = route.legs();
    for (auto const &leg : legs)
    {
        auto const &segment = leg.segments().front();
        if (segment.is_transit())
        {
            auto const &transit = segment.as_transit();
            write_waypoint(oss, transit.stops().front().id());
        }
        else if (segment.is_walk())
        {
            auto const &walk = segment.as_walk();
            write_waypoint(oss, walk.origin());
        }
    }
    oss << ",";
    {
        auto const &segment = legs.back().segments().back();
        if (segment.is_transit())
        {
            auto const &transit = segment.as_transit();
            write_waypoint(oss, transit.stops().back().id());
        }
        else if (segment.is_walk())
        {
            auto const &walk = segment.as_walk();
            write_waypoint(oss, walk.destination());
        }
    }
    oss << "]";
}

void OSRM::write_route(std::ostringstream &oss, navigation::Route const &route) const
{
    auto route_range = route.legs();
    auto total_duration =
        std::accumulate(route_range.begin(), route_range.end(), 0, [](auto accu, auto const &leg) {
            return accu + leg.duration();
        });
    auto total_distance =
        std::accumulate(route_range.begin(), route_range.end(), 0, [](auto accu, auto const &leg) {
            return accu + std::distance(leg.segments().begin(), leg.segments().end());
        });
    oss << "{";
    oss << "\"distance\": " << total_distance << ",";
    oss << "\"duration\": " << total_duration << ",";
    oss << "\"weight\": " << total_duration << ",";
    oss << "\"weight_name\": \"duration\",";
    oss << "\"geometry\": \"";
    write_coordinates(oss, route);
    oss << "\",";
    oss << "\"legs\":[";
    write_legs(oss, route);
    oss << "]}";
}

OSRM::OSRM(StopInfoTable const &stop_info,
           transit::tool::container::StringTable const &dictionary,
           Geometry const &geometry)
    : stop_info(stop_info), dictionary(dictionary), geometry(geometry)
{
}

// annotate a route with all required information. This should be replaced with a json result/what
// ever we want for our api.
std::string OSRM::operator()(navigation::Route const &route) const
{
    std::ostringstream oss;
    oss << "{\"code\": \"Ok\",\"routes\": [";
    write_route(oss, route);
    oss << "],";
    write_waypoints(oss, route);
    oss << "}";
    return oss.str();
}

} // namespace annotation
} // namespace transit
