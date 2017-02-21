#ifndef TRANSIT_GTFS_TRIP_HPP_
#define TRANSIT_GTFS_TRIP_HPP_

#include <boost/optional.hpp>
#include <map>
#include <string>
#include <vector>

#include "gtfs/accessibility.hpp"
#include "gtfs/route.hpp"
#include "gtfs/service.hpp"
#include "gtfs/shape.hpp"
#include "tool/types_and_traits/strong_typedef.hpp"

namespace transit
{
namespace gtfs
{

STRONG_ID_TYPE(std::uint64_t, TripID)
STRONG_ID_TYPE(std::uint64_t, BlockID)

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
    boost::optional<std::string> headsign;
    boost::optional<std::string> short_name;
    boost::optional<TripDirection> direction;
    boost::optional<BlockID> block_id; // same block == transfer without changing cars
    boost::optional<ShapeID> shape_id;
    boost::optional<accessibility::Wheelchair> wheelchair_ccessability;
    boost::optional<BikeTransport> allows_bikes;
};

bool checkTripCSVHeader(std::map<std::string, std::size_t> const &header);
Trip makeTrip(std::map<std::string, std::size_t> const &header, std::vector<std::string> &values);

} // namespace gtfs
} // namespace transit

// needs to be outside of namespaces
STRONG_ID_TYPE_HASH(std::uint64_t, transit::gtfs::TripID)
STRONG_ID_TYPE_HASH(std::uint64_t, transit::gtfs::BlockID)
#endif // TRANSIT_GTFS_TRIP_HPP_
