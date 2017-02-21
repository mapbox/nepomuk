#include "gtfs/trip.hpp"
#include "gtfs/constructor.hpp"

#include <boost/assert.hpp>
#include <boost/optional.hpp>

namespace transit
{
namespace gtfs
{

bool checkTripCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("route_id") && header.count("service_id") && header.count("trip_id");
}

Trip makeTrip(std::map<std::string, std::size_t> const &header, std::vector<std::string> &values)
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

    const auto direction_str =
        construct<boost::optional<std::string>>("direction_id", asOptionalString, header, values);
    const boost::optional<TripDirection> direction =
        direction_str
            ? boost::optional<TripDirection>(to_direction(*direction_str))
            : ((header.count("direction_id")) ? boost::optional<TripDirection>(to_direction(""))
                                              : boost::none);

    const auto wheelchair_optional = construct<boost::optional<std::string>>(
        "wheelchair_boarding", asOptionalString, header, values);
    const boost::optional<accessibility::Wheelchair> wheelchair =
        wheelchair_optional ? boost::optional<accessibility::Wheelchair>(
                                  accessibility::makeWheelchair(*wheelchair_optional))
                            : boost::none;

    const auto bike_str =
        construct<boost::optional<std::string>>("bikes_allowed", asOptionalString, header, values);
    const boost::optional<BikeTransport> bike =
        bike_str ? boost::optional<BikeTransport>(to_bike(*bike_str)) : boost::none;

    return {
        construct<TripID>("trip_id", stringToID<TripID>, header, values),
        construct<RouteID>("route_id", stringToID<RouteID>, header, values),
        construct<ServiceID>("service_id", stringToID<ServiceID>, header, values),
        construct<boost::optional<std::string>>("trip_headsign", asOptionalString, header, values),
        construct<boost::optional<std::string>>(
            "trip_short_name", asOptionalString, header, values),
        direction,
        construct<boost::optional<BlockID>>(
            "block_id", stringToOptionalID<BlockID>, header, values),
        construct<boost::optional<ShapeID>>(
            "shape_id", stringToOptionalID<ShapeID>, header, values),
        wheelchair,
        bike};
}

} // namespace gtfs
} // namespace transit
