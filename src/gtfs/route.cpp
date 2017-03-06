#include "gtfs/route.hpp"
#include "gtfs/constructor.hpp"

#include <algorithm>

#include <boost/optional.hpp>

namespace transit
{
namespace gtfs
{

bool checkRouteCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("route_id") && header.count("route_short_name") &&
           header.count("route_long_name") && header.count("route_type");
}

Route makeRoute(std::map<std::string, std::size_t> const &header,
                std::vector<std::string> &values,
                tool::container::Dictionary &dictionary)
{
    return {
        construct<RouteID>("route_id", stringToID<RouteID>, header, values),
        construct<tool::container::DictionaryID>(
            "route_short_name", DictionaryConverter(dictionary), header, values),
        construct<tool::container::DictionaryID>(
            "route_long_name", DictionaryConverter(dictionary), header, values),
        construct<std::uint64_t>("route_type", toInt, header, values),
        construct_as_optional<AgencyID, false>("agency_id", stringToID<AgencyID>, header, values),
        construct_as_optional<tool::container::DictionaryID, false>(
            "route_desc", DictionaryConverter(dictionary), header, values),
        construct_as_optional<tool::container::DictionaryID, false>(
            "route_url", DictionaryConverter(dictionary), header, values),
        construct_as_optional<std::string, false>("route_color", forward, header, values),
        construct_as_optional<std::string, false>("route_text_color", forward, header, values)};
}

} // namespace gtfs
} // namespace transit
