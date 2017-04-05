#ifndef TRANSIT_GTFS_ROUTE_HPP_
#define TRANSIT_GTFS_ROUTE_HPP_

#include <boost/optional.hpp>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "id/agency.hpp"
#include "id/dictionary.hpp"
#include "tool/container/dictionary.hpp"
#include "id/route.hpp"

namespace transit
{
namespace gtfs
{

// Route types should probably support the extended GTFS format (see
// https://github.com/mapbox/directions-transit/issues/16)

struct Route
{
    RouteID id;
    DictionaryID short_name;
    DictionaryID long_name;
    std::uint64_t type;

    using HexColorCode = std::string;

    // optional additional features
    boost::optional<AgencyID> agency_id;
    boost::optional<DictionaryID> description;
    boost::optional<DictionaryID> url;
    boost::optional<HexColorCode> color;
    boost::optional<HexColorCode> text_color;
};

bool checkRouteCSVHeader(std::map<std::string, std::size_t> const &header);
Route makeRoute(std::map<std::string, std::size_t> const &header,
                std::vector<std::string> &values,
                tool::container::Dictionary &dictionary);

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_ROUTE_HPP_
