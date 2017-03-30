#include "annotation/osrm.hpp"
#include "geometric/coordinate.hpp"
#include "geometric/polyline.hpp"

#include <numeric>
#include <sstream>

// TODO remove
#include <iomanip>
#include <iostream>

namespace transit
{
namespace annotation
{

void OSRM::write_maneuver(std::ostringstream &oss, StopID const stop, std::string const &type) const
{
    oss << "\"maneuver\":{\"location\": [";
    auto const location = geometry.get(stop);
    oss << std::setprecision(12) << doubleFromLatLon(location.longitude) << ","
        << doubleFromLatLon(location.latitude);
    oss << "],\"type\": \"" << type << "\"}";
}

void OSRM::write_steps(std::ostringstream &oss, navigation::Leg const &leg) const
{
    bool print = false;
    auto range = leg.list();

    if (range.size() == 1)
        return;

    auto const constexpr POLYLINE_PRECISION = 100000;
    // print departure
    {
        oss << "{";
        oss << "\"geometry\": \"";
        auto crange = geometry.get(leg.line(), range.front().stop_id, (range.begin() + 1)->stop_id);
        std::vector<geometric::WGS84Coordinate> coordinates(crange.begin(), crange.end());
        oss << geometric::Polyline::encode(POLYLINE_PRECISION, coordinates);
        oss << "\",";
        oss << "\"mode\": \"transit\",";
        oss << "\"name\": \""
            << dictionary.get_string(stop_info.get_info(range.front().stop_id).name_id) << "\",";
        oss << "\"distance\": 1,";
        /*
        oss << "\"duration\": " << leg.duration() << ",";
        oss << "\"weight\": " << leg.duration();
        */
        write_maneuver(oss, range.front().stop_id, "board");
        oss << "}";
    }

    if (range.size() > 3)
    {
        for (auto itr = range.begin() + 1; itr + 1 != range.end(); ++itr)
        {
            // only print at intermediate parts
            oss << ",";
            oss << "{";
            oss << "\"geometry\": \"";
            auto crange = geometry.get(leg.line(), itr->stop_id, (itr + 1)->stop_id);
            std::vector<geometric::WGS84Coordinate> coordinates(crange.begin(), crange.end());
            oss << geometric::Polyline::encode(POLYLINE_PRECISION, coordinates);
            oss << "\",";
            oss << "\"mode\": \"transit\",";
            oss << "\"name\": \"" << dictionary.get_string(stop_info.get_info(itr->stop_id).name_id)
                << "\",";
            oss << "\"distance\": 1,";
            /*
            oss << "\"duration\": " << leg.duration() << ",";
            oss << "\"weight\": " << leg.duration();
            */
            write_maneuver(oss, itr->stop_id, "stay in vehicle");
            oss << "}";
        }
    }
    {
        // only print at intermediate parts
        oss << ",";
        oss << "{";
        oss << "\"mode\": \"transit\",";
        oss << "\"name\": \""
            << dictionary.get_string(stop_info.get_info(range.back().stop_id).name_id) << "\",";
        oss << "\"distance\": 1,";
        /*
        oss << "\"duration\": " << leg.duration() << ",";
        oss << "\"weight\": " << leg.duration();
        */
        write_maneuver(oss, range.back().stop_id, "deboard");
        oss << "}";
    }
}

void OSRM::write_leg(std::ostringstream &oss, navigation::Leg const &leg) const
{
    oss << "{";
    oss << "\"distance\": " << leg.size() << ",";
    oss << "\"duration\": " << leg.duration() << ",";
    oss << "\"weight\": " << leg.duration() << ",";
    oss << "\"summary\": "
        << "\"LINE_NAME_HERE\","; // TODO
    oss << "\"steps\":[";
    write_steps(oss, leg);
    oss << "]";
    oss << "}";
}

void OSRM::write_legs(std::ostringstream &oss, navigation::Trip const &trip) const
{
    bool start_with_comma = false;
    // Write out information about a trip
    for (auto const &leg : trip.list())
    {
        if (start_with_comma)
            oss << ",";
        write_leg(oss, leg);
        start_with_comma = true;
    }
}

void OSRM::write_coordinates(std::ostringstream &oss, navigation::Trip const &trip) const
{
    bool start_with_comma = false;
    std::vector<geometric::WGS84Coordinate> coordinates;
    // Write out information about a trip
    for (auto const &leg : trip.list())
    {
        auto stops = leg.list();
        auto crange = geometry.get(leg);
        coordinates.insert(coordinates.end(), crange.begin(), crange.end());
    }

    oss << geometric::Polyline::encode(100000, coordinates);
}

void OSRM::write_trip(std::ostringstream &oss, navigation::Trip const &trip) const
{
    auto trip_range = trip.list();
    auto total_duration =
        std::accumulate(trip_range.begin(), trip_range.end(), 0, [](auto accu, auto const &leg) {
            return accu + leg.duration();
        });
    auto total_distance =
        std::accumulate(trip_range.begin(), trip_range.end(), 0, [](auto accu, auto const &leg) {
            return accu + leg.size();
        });
    oss << "{";
    oss << "\"distance\": " << total_distance << ",";
    oss << "\"duration\": " << total_duration << ",";
    oss << "\"weight\": " << total_duration << ",";
    oss << "\"weight_name\": \"duration\",";
    oss << "\"geometry\": \"";
    write_coordinates(oss, trip);
    oss << "\",";
    oss << "\"legs\":[";
    write_legs(oss, trip);
    oss << "]}";
}

OSRM::OSRM(StopInfoTable const &stop_info,
           transit::tool::container::StringTable const &dictionary,
           Geometry const &geometry)
    : stop_info(stop_info), dictionary(dictionary), geometry(geometry)
{
}

// annotate a trip with all required information. This should be replaced with a json result/what
// ever we want for our api.
std::string OSRM::operator()(navigation::Trip const &trip) const
{
    std::ostringstream oss;
    oss << "{\"code\": \"Ok\",\"routes\": [";
    write_trip(oss, trip);
    oss << "]}";
    return oss.str();
}

} // namespace annotation
} // namespace transit
