#include "gtfs/trip.hpp"
#include "gtfs/constructor.hpp"

#include <boost/assert.hpp>

namespace nepomuk
{
namespace gtfs
{

bool checkTripCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("route_id") && header.count("service_id") && header.count("trip_id");
}

Trip makeTrip(std::map<std::string, std::size_t> const &header,
              std::vector<std::string> &values,
              tool::container::Dictionary &dictionary)
{
    auto const to_direction = [](std::string const &value) {
        if (value.empty())
            return TripDirection::UNKNOWN;
        else if (value == "0")
            return TripDirection::INBOUND;
        else
        {
            BOOST_ASSERT(value == "1");
            return TripDirection::OUTBOUND;
        }
    };

    auto const to_bike = [](std::string const &value) {
        if (value == "1")
            return BikeTransport::ALLOWED;
        else if (value == "2")
            return BikeTransport::FORBIDDEN;
        else
        {
            BOOST_ASSERT(value.empty() || value == "0");
            return BikeTransport::UNKNOWN;
        }
    };

    return {
        construct<TripID>("trip_id", stringToID<TripID>, header, values),
        construct<RouteID>("route_id", stringToID<RouteID>, header, values),
        construct<ServiceID>("service_id", stringToID<ServiceID>, header, values),
        construct_as_optional<DictionaryID, false>(
            "trip_headsign", DictionaryConverter(dictionary), header, values),
        construct_as_optional<DictionaryID, false>(
            "trip_short_name", DictionaryConverter(dictionary), header, values),
        construct_as_optional<TripDirection, true>("direction_id", to_direction, header, values),
        construct_as_optional<BlockID, false>("block_id", stringToID<BlockID>, header, values),
        construct_as_optional<ShapeID, false>("shape_id", stringToID<ShapeID>, header, values),
        construct_as_optional<accessibility::Wheelchair, true>(
            "wheelchair_boarding", accessibility::makeWheelchair, header, values),
        construct_as_optional<BikeTransport, true>("bikes_allowed", to_bike, header, values)};
}

} // namespace gtfs
} // namespace nepomuk
