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

Route makeRoute(std::map<std::string, std::size_t> const &header, std::vector<std::string> &values)
{
    return {
        construct<RouteID>("route_id", stringToID<RouteID>, header, values),
        construct<std::string>("route_short_name", forward, header, values),
        construct<std::string>("route_long_name", forward, header, values),
        construct<std::uint64_t>("route_type", toInt, header, values),
        construct<boost::optional<std::string>>("agency_id", asOptionalString, header, values),
        construct<boost::optional<std::string>>("route_desc", asOptionalString, header, values),
        construct<boost::optional<std::string>>("route_url", asOptionalString, header, values),
        construct<boost::optional<std::string>>("route_color", asOptionalString, header, values),
        construct<boost::optional<std::string>>(
            "route_text_color", asOptionalString, header, values)};
}

} // namespace gtfs
} // namespace transit
