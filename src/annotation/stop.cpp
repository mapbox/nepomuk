#include "annotation/stop.hpp"

#include <algorithm>

#include "gtfs/stop.hpp"

namespace nepomuk
{
namespace annotation
{

Stop::Stop(std::vector<gtfs::Stop> const &stops)
{
    stop_info.resize(stops.size());

    // set the names
    std::transform(stops.begin(),
                   stops.end(),
                   stop_info.begin(),
                   [this](auto const &stop) -> StopInfo { return {stop.name}; });
}

StopInfo const &Stop::operator()(StopID const stop_id) const { return stop_info[stop_id.base()]; }

} // namespace annotation
} // namespace nepomuk
