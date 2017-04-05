#include "gtfs/dataset.hpp"
#include "algorithm/ranges.hpp"
#include "tool/status/progress.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

#include <boost/assert.hpp>
#include <boost/optional.hpp>
#include <boost/range/iterator_range.hpp>

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
    tool::status::FunctionTimingGuard guard("filtering unreachable stops");
    // create look-up of all reachable stop_ids
    auto reachable_stops = get_reachable_stops(stop_times);

    add_stations(reachable_stops, stops);

    if (transfers)
        add_transfers(reachable_stops, *transfers);

    auto const unreachable = [&reachable_stops](auto const &stop) {
        return reachable_stops.count(stop.id) == 0;
    };

    std::cout << "[filtering unreachable stops]: " << stops.size();
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

void Dataset::connect_stops_into_stations(std::uint32_t const proximity_requirement)
{
    tool::status::FunctionTimingGuard guard("stops into stations");
    std::unordered_multimap<DictionaryID, std::size_t> stops_by_id;
    std::for_each(stops.begin(), stops.end(), [&, pos = std::size_t{0} ](auto const &stop) mutable {
        stops_by_id.insert(std::make_pair(stop.name, pos++));
    });

    auto const group_close_points = [&](auto bucket) {
        if (std::distance(bucket.begin(), bucket.end()) > 1)
        {
            auto const has_parent_station = [&](auto const &pair_name_offset) -> bool {
                return stops[pair_name_offset.second].parent_station != boost::none;
            };
            if (std::any_of(bucket.begin(), bucket.end(), has_parent_station))
                return;

            auto reference_index =
                std::min_element(bucket.begin(), bucket.end(), [](auto const lhs, auto const rhs) {
                    return lhs.second < rhs.second;
                })->second;

            auto const reference_position = stops[reference_index].location;
            auto const are_close = [&](auto const &pair_name_offset) {
                return geometric::distance(reference_position,
                                           stops[pair_name_offset.second].location) <=
                       proximity_requirement;
            };
            if (std::all_of(bucket.begin(), bucket.end(), are_close))
            {
                // all elements are within the proximity distance to the first stop, make it a
                // station. This is kind of dirty, we should calculate the center of the
                // point cloud and check for half the proximity requirement
                auto reference = stops[reference_index].id;
                auto const assign_parent = [&](auto const &pair_name_offset) {
                    if (stops[pair_name_offset.second].id != reference)
                        stops[pair_name_offset.second].parent_station = reference;
                };
                std::for_each(bucket.begin(), bucket.end(), assign_parent);
            }
        }
    };

    for (std::size_t bucket_id = 0; bucket_id < stops_by_id.bucket_count(); ++bucket_id)
        group_close_points(
            boost::make_iterator_range(stops_by_id.begin(bucket_id), stops_by_id.end(bucket_id)));
}

} // namespace gtfs
} // namespace transit
