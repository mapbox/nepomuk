#include "annotation/stop_info.hpp"

#include <algorithm>

#include "gtfs/stop.hpp"

namespace nepomuk
{
namespace annotation
{

StopInfoTable::StopInfoTable(std::vector<gtfs::Stop> const &stops)
{
    stop_info.resize(stops.size());

    // set the names
    std::transform(stops.begin(),
                   stops.end(),
                   stop_info.begin(),
                   [this](auto const &stop) -> StopInfo { return {stop.name}; });
}

StopInfo const &StopInfoTable::get_info(StopID const stop_id) const
{
    return stop_info[stop_id.base()];
}

} // namespace annotation
} // namespace nepomuk
