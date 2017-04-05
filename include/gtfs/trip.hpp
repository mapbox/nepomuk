#ifndef TRANSIT_GTFS_TRIP_HPP_
#define TRANSIT_GTFS_TRIP_HPP_

#include <boost/optional.hpp>
#include <map>
#include <string>
#include <vector>

#include "gtfs/accessibility.hpp"
#include "id/block.hpp"
#include "id/dictionary.hpp"
#include "id/route.hpp"
#include "id/service.hpp"
#include "id/shape.hpp"
#include "id/trip.hpp"
#include "tool/container/dictionary.hpp"

namespace transit
{
namespace gtfs
{

enum class TripDirection
{
    INBOUND,
    OUTBOUND,
    UNKNOWN
};

enum class BikeTransport
{
    UNKNOWN,
    ALLOWED,
    FORBIDDEN
};

struct Trip
{
    TripID id;
    RouteID route_id;
    ServiceID service_id;

    // optional additional features
    boost::optional<DictionaryID> headsign;
    boost::optional<DictionaryID> short_name;
    boost::optional<TripDirection> direction;
    boost::optional<BlockID> block_id; // same block == transfer without changing cars
    boost::optional<ShapeID> shape_id;
    boost::optional<accessibility::Wheelchair> wheelchair_ccessability;
    boost::optional<BikeTransport> allows_bikes;
};

bool checkTripCSVHeader(std::map<std::string, std::size_t> const &header);
Trip makeTrip(std::map<std::string, std::size_t> const &header,
              std::vector<std::string> &values,
              tool::container::Dictionary &dictionary);

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_TRIP_HPP_
