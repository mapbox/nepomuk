#ifndef TRANSIT_GTFS_ROUTE_HPP_
#define TRANSIT_GTFS_ROUTE_HPP_

#include <boost/optional.hpp>

#include <cstdint>
#include <map>
#include <string>

#include "gtfs/agency.hpp"
#include "tool/container/dictionary.hpp"
#include "tool/types_and_traits/strong_typedef.hpp"

namespace transit
{
namespace gtfs
{

STRONG_ID_TYPE(std::uint64_t, RouteID)

// Route types should probably support the extended GTFS format (see
// https://github.com/mapbox/directions-transit/issues/16)

struct Route
{
    RouteID id;
    tool::container::DictionaryID short_name;
    tool::container::DictionaryID long_name;
    std::uint64_t type;

    using HexColorCode = std::string;

    // optional additional features
    boost::optional<AgencyID> agency_id;
    boost::optional<tool::container::DictionaryID> description;
    boost::optional<tool::container::DictionaryID> url;
    boost::optional<HexColorCode> color;
    boost::optional<HexColorCode> text_color;
};

bool checkRouteCSVHeader(std::map<std::string, std::size_t> const &header);
Route makeRoute(std::map<std::string, std::size_t> const &header,
                std::vector<std::string> &values,
                tool::container::Dictionary &dictionary);

} // namespace gtfs
} // namespace transit

// needs to be outside of namespaces
STRONG_ID_TYPE_HASH(std::uint64_t, transit::gtfs::RouteID)
#endif // TRANSIT_GTFS_ROUTE_HPP_
