#ifndef TRANSIT_GTFS_STOP_HPP_
#define TRANSIT_GTFS_STOP_HPP_

#include <boost/optional.hpp>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "gtfs/accessibility.hpp"
#include "gtfs/sequence.hpp"
#include "gtfs/time.hpp"
#include "gtfs/trip.hpp"
#include "gtfs/zone.hpp"
#include "tool/container/dictionary.hpp"
#include "tool/types_and_traits/strong_typedef.hpp"

namespace transit
{
namespace gtfs
{

STRONG_ID_TYPE(std::uint64_t, StopID)

enum class LocationType
{
    STOP,
    STATION
};

struct Stop
{
    StopID id;
    tool::container::DictionaryID name;
    geometric::WGS84Coordinate location;

    // optional additional features
    boost::optional<std::string> code;
    boost::optional<tool::container::DictionaryID> description;
    boost::optional<ZoneID> zone_id;
    boost::optional<tool::container::DictionaryID> url;
    boost::optional<LocationType> location_type;
    boost::optional<StopID> parent_station;
    boost::optional<std::string> timezone;
    boost::optional<accessibility::Wheelchair> wheelchair_access;
};

bool checkStopCSVHeader(std::map<std::string, std::size_t> const &header);
Stop makeStop(std::map<std::string, std::size_t> const &header,
              std::vector<std::string> &values,
              tool::container::Dictionary &dictionary);

enum class StopType
{
    REGULAR,
    NOT_AVAILABLE,
    PHONE_AGENCY,
    COORDINATE_DRIVER
};

enum class TimepointType
{
    EXACT,
    APPROXIMATE
};

// noon - 12 times
struct StopTime
{
    TripID trip_id;
    Time arrival;
    Time departure;
    StopID stop_id;
    SequenceID sequence_id;

    // optional additional features
    boost::optional<std::string> headsign;
    boost::optional<StopType> pickup_type;
    boost::optional<StopType> drop_off_type;
    boost::optional<double> shape_distance_traveled;
    boost::optional<TimepointType> timepoint;
};

bool checkStopTimeCSVHeader(std::map<std::string, std::size_t> const &header);
StopTime makeStopTime(std::map<std::string, std::size_t> const &header,
                      std::vector<std::string> &values);

} // namespace gtfs
} // namespace transit

// needs to be outside of namespaces
STRONG_ID_TYPE_HASH(std::uint64_t, transit::gtfs::StopID)
#endif // TRANSIT_GTFS_STOP_HPP_
