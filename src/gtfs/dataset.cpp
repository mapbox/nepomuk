#include "gtfs/dataset.hpp"
#include "tool/status/progress.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_set>

#include <boost/assert.hpp>
#include <boost/optional.hpp>

namespace transit
{
namespace gtfs
{

namespace
{

std::unordered_set<StopID> get_reachable_stops(std::vector<StopTime> const &stop_times)
{
    std::unordered_set<StopID> reachable_stops;
    std::transform(stop_times.begin(),
                   stop_times.end(),
                   std::inserter(reachable_stops, reachable_stops.end()),
                   [](auto const &stop_time) { return stop_time.stop_id; });

    return reachable_stops;
}

void add_transfers(std::unordered_set<StopID> &reachable_stops,
                   std::vector<Transfer> const &transfers)
{
    std::transform(transfers.begin(),
                   transfers.end(),
                   std::inserter(reachable_stops, reachable_stops.end()),
                   [](auto const &transfer) { return transfer.from; });

    std::transform(transfers.begin(),
                   transfers.end(),
                   std::inserter(reachable_stops, reachable_stops.end()),
                   [](auto const &transfer) { return transfer.to; });
}

void add_stations(std::unordered_set<StopID> &reachable_stops, std::vector<Stop> const &stops)
{
    auto const add_station_on_reachable_stop = [&](auto const &stop) {
        if (stop.parent_station && *stop.parent_station != stop.id)
            reachable_stops.insert(*stop.parent_station);
    };

    std::for_each(stops.begin(), stops.end(), add_station_on_reachable_stop);
}

std::unordered_map<StopID, StopID> remap(std::vector<Stop> &stops)
{
    std::unordered_map<StopID, StopID> mapping;
    StopID::value_type continuous_id = 0;

    auto const remap_stop = [&](auto &stop) { mapping[stop.id] = StopID{continuous_id++}; };

    std::for_each(stops.begin(), stops.end(), remap_stop);
    return mapping;
}

void apply_mapping(std::unordered_map<StopID, StopID> const &mapping, StopID &id)
{
    BOOST_ASSERT(mapping.count(id));
    id = mapping.find(id)->second;
}

}

void Dataset::filter_unreachable_stops()
{
    tool::status::FunctionTimingGuard guard("filtering unreachable nodes");
    // create look-up of all reachable stop_ids
    auto reachable_stops = get_reachable_stops(stop_times);

    add_stations(reachable_stops, stops);

    if (transfers)
        add_transfers(reachable_stops, *transfers);

    auto const unreachable = [&reachable_stops](auto const &stop) {
        return reachable_stops.count(stop.id) == 0;
    };

    std::cout << "[filtering unreachable nodes]: " << stops.size();
    stops.erase(std::remove_if(stops.begin(), stops.end(), unreachable), stops.end());
    std::cout << " -> " << stops.size() << " stops." << std::endl;

    // create a new mapping of the stop-ids
    auto const mapping = remap(stops);

    // remap parent stations
    std::for_each(stops.begin(), stops.end(), [&](auto &stop) {
        apply_mapping(mapping, stop.id);
        if (stop.parent_station)
            apply_mapping(mapping, stop.parent_station.get());
    });

    // remap stop times
    std::for_each(stop_times.begin(), stop_times.end(), [&](auto &stop_time) {
        apply_mapping(mapping, stop_time.stop_id);
    });

    // remap transfers
    if (transfers)
    {
        std::for_each(transfers->begin(), transfers->end(), [&](auto &transfer) {
            apply_mapping(mapping, transfer.from);
            apply_mapping(mapping, transfer.to);
        });
    }
}

} // namespace gtfs
} // namespace transit
