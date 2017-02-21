#include "gtfs/stop.hpp"
#include "gtfs/constructor.hpp"

#include <algorithm>
#include <boost/assert.hpp>
#include <boost/optional.hpp>

namespace transit
{
namespace gtfs
{

bool checkStopCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("stop_id") && header.count("stop_name") && header.count("stop_lat") &&
           header.count("stop_lon");
}

Stop makeStop(std::map<std::string, std::size_t> const &header, std::vector<std::string> &values)
{
    auto const to_type = [](std::string const &value) {
        if (value == "1")
            return LocationType::STATION;
        else
        {
            BOOST_ASSERT(value.empty() || value == "0");
            return LocationType::STOP;
        }
    };

    const double lat = std::stof(construct<std::string>("stop_lat", forward, header, values));
    const double lon = std::stof(construct<std::string>("stop_lon", forward, header, values));

    const auto type_optional =
        construct<boost::optional<std::string>>("location_type", asOptionalString, header, values);
    boost::optional<LocationType> type =
        header.count("location_type")
            ? (type_optional ? boost::optional<LocationType>(to_type(*type_optional))
                             : boost::optional<LocationType>(to_type("")))
            : boost::none;

    const auto wheelchair_optional = construct<boost::optional<std::string>>(
        "wheelchair_boarding", asOptionalString, header, values);
    const boost::optional<accessibility::Wheelchair> wheelchair =
        wheelchair_optional ? boost::optional<accessibility::Wheelchair>(
                                  accessibility::makeWheelchair(*wheelchair_optional))
                            : boost::none;

    return {
        construct<StopID>("stop_id", stringToID<StopID>, header, values),
        construct<std::string>("stop_name", forward, header, values),
        geometric::Coordinate(geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
                              geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat)),
        construct<boost::optional<std::string>>("stop_code", asOptionalString, header, values),
        construct<boost::optional<std::string>>("stop_desc", asOptionalString, header, values),
        construct<boost::optional<ZoneID>>("zone_id", stringToOptionalID<ZoneID>, header, values),
        construct<boost::optional<std::string>>("stop_url", asOptionalString, header, values),
        type,
        construct<boost::optional<StopID>>(
            "parent_station", stringToOptionalID<StopID>, header, values),
        construct<boost::optional<std::string>>("stop_timezone", asOptionalString, header, values),
        wheelchair};
}

bool checkStopTimeCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("trip_id") && header.count("arrival_time") &&
           header.count("departure_time") && header.count("stop_id") &&
           header.count("stop_sequence");
}

StopTime makeStopTime(std::map<std::string, std::size_t> const &header,
                      std::vector<std::string> &values)
{
    auto const to_type = [](std::string const &value) {
        if (value == "1")
            return StopType::NOT_AVAILABLE;
        else if (value == "2")
            return StopType::PHONE_AGENCY;
        else if (value == "3")
            return StopType::COORDINATE_DRIVER;
        else
        {
            BOOST_ASSERT(value.empty() || value == "0");
            return StopType::REGULAR;
        }
    };

    auto const to_point = [](std::string const &value) {
        if (value == "0")
            return TimepointType::APPROXIMATE;
        else
        {
            BOOST_ASSERT(value.empty() || value == "1");
            return TimepointType::EXACT;
        }
    };

    const auto pickup_str =
        construct<boost::optional<std::string>>("pickup_type", asOptionalString, header, values);
    const auto drop_off_str =
        construct<boost::optional<std::string>>("drop_off_type", asOptionalString, header, values);

    boost::optional<StopType> pickup =
        pickup_str ? boost::optional<StopType>(to_type(*pickup_str)) : boost::none;
    boost::optional<StopType> drop_off =
        drop_off_str ? boost::optional<StopType>(to_type(*drop_off_str)) : boost::none;

    const auto dist_travelled_str = construct<boost::optional<std::string>>(
        "shape_dist_traveled", asOptionalString, header, values);
    const auto timepoint_str =
        construct<boost::optional<std::string>>("timepoint", asOptionalString, header, values);

    boost::optional<double> dist_travelled =
        dist_travelled_str ? boost::optional<double>(std::stof(*dist_travelled_str)) : boost::none;
    boost::optional<TimepointType> timepoint =
        timepoint_str ? boost::optional<TimepointType>(to_point(*timepoint_str)) : boost::none;

    return {
        construct<TripID>("trip_id", stringToID<TripID>, header, values),
        construct<Time>("arrival_time", constructFromString<Time>, header, values),
        construct<Time>("departure_time", constructFromString<Time>, header, values),
        construct<StopID>("stop_id", stringToID<StopID>, header, values),
        construct<SequenceID>("stop_sequence", stringToID<SequenceID>, header, values),
        construct<boost::optional<std::string>>("stop_headsign", asOptionalString, header, values),
        pickup,
        drop_off,
        dist_travelled,
        timepoint};
}

} // namespace gtfs
} // namespace transit
