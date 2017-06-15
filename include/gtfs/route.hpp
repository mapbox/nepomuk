#ifndef NEPOMUK_GTFS_ROUTE_HPP_
#define NEPOMUK_GTFS_ROUTE_HPP_

#include <boost/optional.hpp>

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "id/agency.hpp"
#include "id/dictionary.hpp"
#include "id/route.hpp"

namespace nepomuk
{

namespace tool
{
namespace container
{
class Dictionary;
}
}

namespace gtfs
{

// Route types should probably support the extended GTFS format (see
// https://github.com/mapbox/nepomuk/issues/16)

struct Route
{
    RouteID id;
    DictionaryID short_name;
    DictionaryID long_name;
    std::uint64_t type;

    // optional additional features
    boost::optional<AgencyID> agency_id;
    boost::optional<DictionaryID> description;
    boost::optional<DictionaryID> url;
    boost::optional<DictionaryID> color;
    boost::optional<DictionaryID> text_color;
};

bool checkRouteCSVHeader(std::map<std::string, std::size_t> const &header);
Route makeRoute(std::map<std::string, std::size_t> const &header,
                std::vector<std::string> &values,
                tool::container::Dictionary &dictionary);

} // namespace gtfs
} // namespace nepomuk

#endif // NEPOMUK_GTFS_ROUTE_HPP_
