#include "gtfs/stop.hpp"
#include "gtfs/constructor.hpp"

#include <boost/assert.hpp>

#include "date/timezone.hpp"

namespace nepomuk
{
namespace gtfs
{

bool checkStopCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("stop_id") && header.count("stop_name") && header.count("stop_lat") &&
           header.count("stop_lon");
}

Stop makeStop(std::map<std::string, std::size_t> const &header,
              std::vector<std::string> &values,
              tool::container::Dictionary &dictionary)
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

    const double lat = std::stod(construct<std::string>("stop_lat", forward, header, values));
    const double lon = std::stod(construct<std::string>("stop_lon", forward, header, values));

    auto const as_utc_offset = [](std::string const& timezone)
    {
        auto offset = date::Timezone::offset(timezone);
        BOOST_ASSERT(offset);
        return *offset;
    };

    return {
        construct<StopID>("stop_id", stringToID<StopID>, header, values),
        construct<DictionaryID>("stop_name", DictionaryConverter(dictionary), header, values),
        geometric::WGS84Coordinate(geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
                                   geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat)),
        construct_as_optional<std::string, false>("stop_code", forward, header, values),
        construct_as_optional<DictionaryID, false>(
            "stop_desc", DictionaryConverter(dictionary), header, values),
        construct_as_optional<ZoneID, false>("zone_id", stringToID<ZoneID>, header, values),
        construct_as_optional<DictionaryID, false>(
            "stop_url", DictionaryConverter(dictionary), header, values),
        construct_as_optional<LocationType, true>("location_type", to_type, header, values),
        construct_as_optional<StopID, false>("parent_station", stringToID<StopID>, header, values),
        construct_as_optional<std::int32_t, false>("stop_timezone", as_utc_offset, header, values),
        construct_as_optional<accessibility::Wheelchair, true>(
            "wheelchair_boarding", accessibility::makeWheelchair, header, values)};
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

    const auto dist_travelled_str =
        construct_as_optional<std::string, false>("shape_dist_traveled", forward, header, values);
    const auto timepoint_str =
        construct_as_optional<std::string, true>("timepoint", forward, header, values);

    boost::optional<double> dist_travelled =
        dist_travelled_str ? boost::make_optional(std::stod(*dist_travelled_str)) : boost::none;
    boost::optional<TimepointType> timepoint =
        timepoint_str ? boost::optional<TimepointType>(to_point(*timepoint_str)) : boost::none;

    return {
        construct<TripID>("trip_id", stringToID<TripID>, header, values),
        construct<date::Time>("arrival_time", constructFromString<date::Time>, header, values),
        construct<date::Time>("departure_time", constructFromString<date::Time>, header, values),
        construct<StopID>("stop_id", stringToID<StopID>, header, values),
        construct<SequenceID>("stop_sequence", stringToIDDirect<SequenceID>, header, values),
        construct_as_optional<std::string, false>("stop_headsign", forward, header, values),
        construct_as_optional<StopType, true>("pickup_type", to_type, header, values),
        construct_as_optional<StopType, true>("drop_off_type", to_type, header, values),
        dist_travelled,
        construct_as_optional<TimepointType, true>("timepoint", to_point, header, values)};
}

} // namespace gtfs
} // namespace nepomuk
