#include "navigation/algorithm/timetable.hpp"
#include "navigation/leg.hpp"

#include "id/line.hpp"
#include "id/stop.hpp"
#include "id/trip.hpp"

#include <algorithm>
#include <iterator>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

namespace transit
{
namespace navigation
{
namespace algorithm
{

TimeTable::TimeTable(timetable::TimeTable const &time_table, search::StopToLine const &stop_to_line)
    : RoutingAlgorithm(time_table.lines()), time_table(time_table), stop_to_line(stop_to_line)
{
}

boost::optional<Route> TimeTable::
operator()(date::Time const departure, StopID const origin, StopID const destination) const
{
    ReachedStateContainer earliest_arrival;
    StateContainer internal_state;

    add_origin(origin, departure, internal_state, earliest_arrival);

    date::Time upper_bound;
    upper_bound = upper_bound + std::numeric_limits<std::uint32_t>::max();
    StopID reached_destination = destination;
    auto const destination_station = time_table.station(destination);

    auto const check_and_improve_upper_bound = [&](StopID const stop) {
        auto const itr = earliest_arrival.find(stop);
        if (itr != earliest_arrival.end() && itr->second.arrival < upper_bound)
        {
            // std::cout << "Found new upper bound: " << stop << " " << itr->second.arrival
            //          << std::endl;
            upper_bound = itr->second.arrival;
            reached_destination = stop;
        }
    };
    // relax by lines, in order of hops
    auto const &destination_stops = time_table.stops(destination_station);
    std::size_t offset = 0;
    while (offset < internal_state.size())
    {
        // relax all hops
        std::sort(internal_state.begin() + offset,
                  internal_state.end(),
                  [](auto const &lhs, auto const &rhs) { return lhs.arrival < rhs.arrival; });

        std::for_each(
            destination_stops.begin(), destination_stops.end(), check_and_improve_upper_bound);

        auto const new_offset = internal_state.size();
        relax(
            upper_bound, offset, internal_state.size() - offset, internal_state, earliest_arrival);
        offset = new_offset;
    }

    std::for_each(
        destination_stops.begin(), destination_stops.end(), check_and_improve_upper_bound);

    if (!earliest_arrival.count(reached_destination))
        return boost::none;

    return make_route(extract_path(reached_destination, earliest_arrival));
}

boost::optional<Route> TimeTable::operator()(date::Time const departure,
                                             std::vector<ADLeg> const &origins,
                                             std::vector<ADLeg> const &destinations) const
{
    if (origins.empty() || destinations.empty())
        return boost::none;

    ReachedStateContainer earliest_arrival;
    StateContainer internal_state;

    for (auto const &leg : origins)
        add_origin(leg.stop, departure + leg.seconds, internal_state, earliest_arrival);

    std::vector<std::pair<StopID, std::uint32_t>> destination_stations;
    destination_stations.reserve(destinations.size());
    std::transform(destinations.begin(),
                   destinations.end(),
                   std::back_inserter(destination_stations),
                   [this](auto const &adleg) {
                       return std::make_pair(time_table.station(adleg.stop), adleg.seconds);
                   });

    std::sort(destination_stations.begin(), destination_stations.end());
    destination_stations.erase(
        std::unique(destination_stations.begin(),
                    destination_stations.end(),
                    [](auto const &lhs, auto const &rhs) { return lhs.first < rhs.first; }),
        destination_stations.end());

    date::Time upper_bound;
    upper_bound = upper_bound + std::numeric_limits<std::uint32_t>::max();
    StopID reached_destination = destinations.front().stop;
    // make into multiple stations
    auto const check_and_improve_upper_bound = [&](auto const &stop_and_time) {
        auto const itr = earliest_arrival.find(stop_and_time.first);
        if (itr != earliest_arrival.end() &&
            (itr->second.arrival + stop_and_time.second) < upper_bound)
        {
            reached_destination = stop_and_time.first;
        }
    };

    std::size_t offset = 0;
    while (offset < internal_state.size())
    {
        std::sort(internal_state.begin() + offset,
                  internal_state.end(),
                  [](auto const &lhs, auto const &rhs) { return lhs.arrival < rhs.arrival; });

        std::for_each(destination_stations.begin(),
                      destination_stations.end(),
                      check_and_improve_upper_bound);

        auto const new_offset = internal_state.size();
        relax(
            upper_bound, offset, internal_state.size() - offset, internal_state, earliest_arrival);
        offset = new_offset;
    }

    if (!earliest_arrival.count(reached_destination))
        return boost::none;

    return make_route(extract_path(reached_destination, earliest_arrival));
}

void TimeTable::add_origin(StopID const origin,
                           date::Time const departure,
                           StateContainer &internal_state,
                           ReachedStateContainer &reached) const
{
    for (auto start : time_table.stops(time_table.station(origin)))
    {
        if (!reached.count(start) || departure < (reached[start].arrival))
        {
            // std::cout << "[origin] " << start << " at " << departure << std::endl;
            internal_state.push_back({start, departure});
            reached[start] = {departure, start, departure, WALKING_TRANSFER};
        }
    }
}

void TimeTable::relax(date::Time const &upper_bound,
                      std::size_t offset,
                      std::size_t count,
                      StateContainer &internal_state,
                      ReachedStateContainer &earliest_arrival) const
{
    auto const add_if_improved = [&](StopID const stop,
                                     date::Time const time,
                                     StopID const parent,
                                     LineID const line,
                                     date::Time const departure) {
        if (!earliest_arrival.count(stop) || time < earliest_arrival[stop].arrival)
        {
            BOOST_ASSERT(parent != stop);
            earliest_arrival[stop] = {time, parent, departure, line};
            // std::cout << "\t\t[Reach] " << stop << " from " << parent << " at " << time << " via
            // "
            //          << line << std::endl;
            return true;
        }
        else
            return false;
    };

    auto const relax_line = [&](auto const &state, auto const line_id) {
        // std::cout << "-------" << std::endl;
        auto const trip_optional = time_table.line(line_id).get(state.stop_id, state.arrival);
        if (!trip_optional)
            return;

        auto const &trip = *trip_optional;
        auto time = trip.departure;

        auto duration_itr = trip.duration_range.begin();
        for (auto stop_itr = trip.stop_range.begin(); stop_itr != trip.stop_range.end();
             ++stop_itr, ++duration_itr)
        {
            auto const stop_id = *stop_itr;
            // std::cout << "\t\t" << stop_id << " " << time + *duration_itr << std::endl;
            if (add_if_improved(stop_id, time, state.stop_id, line_id, trip.departure))
            {
                auto transfers = time_table.transfers(stop_id);
                // add all transfers
                for (auto transfer : transfers)
                {
                    auto transfer_time = time + std::max<int>(transfer.duration, 60);
                    if (add_if_improved(
                            transfer.stop_id, transfer_time, stop_id, WALKING_TRANSFER, time) ||
                        (transfer.stop_id == stop_id))
                    {
                        // needs to be a transfer line instead of 0
                        internal_state.push_back({transfer.stop_id, time + transfer.duration});
                    }
                }
            }
            time = time + *duration_itr;
        }
    };

    // std::cout << "[Round]" << std::endl;
    while (count--)
    {
        auto const state = internal_state[offset++];

        // std::cout << "\tAt: " << state.stop_id << " " << state.arrival << std::endl;
        // stop when the earliest arrival is known
        if (upper_bound <= state.arrival)
            break; // sorted, all current states will be out of bound

        // get all lines at the given stop
        auto trip_range = stop_to_line(state.stop_id);
        for (auto line : trip_range)
            relax_line(state, line);
    }
}

std::vector<TimeTable::PathEntry>
TimeTable::extract_path(StopID current_stop, ReachedStateContainer const &earliest_arrival) const
{
    std::vector<PathEntry> path;
    auto entry = earliest_arrival.find(current_stop)->second;
    path.push_back({current_stop, entry.line_id, entry.arrival, entry.parent_departure});
    do
    {
        current_stop = entry.parent;
        entry = earliest_arrival.find(current_stop)->second;
        path.push_back({current_stop, entry.line_id, entry.arrival, entry.parent_departure});
    } while (entry.parent != current_stop);

    std::reverse(path.begin(), path.end());
    return path;
}

} // namespace algorithm
} // namespace navigation
} // namespace transit
