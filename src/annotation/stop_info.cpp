#include "annotation/stop_info.hpp"

#include <algorithm>
#include <boost/assert.hpp>
#include <iterator>

namespace transit
{
namespace annotation
{

StopInfoTable::StopInfoTable(std::vector<gtfs::Stop> &stops)
{
    std::sort(stops.begin(), stops.end(), [](auto const &lhs, auto const &rhs) {
        return lhs.id < rhs.id;
    });

    stop_info.reserve(stops.size());

    // add all stop entries to the stop_info vector
    std::transform(stops.begin(),
                   stops.end(),
                   std::back_inserter(stop_info),
                   [this](auto const &stop) -> StopInfo {
                       BOOST_ASSERT(stop_info.size() == static_cast<std::uint64_t>(stop.id));
                       return {stop.name.base(), stop.location};
                   });
}

StopInfo StopInfoTable::get_info(StopID const stop_id) const
{
    return stop_info[stop_id.base()];
}

} // namespace annotation
} // namespace transit
