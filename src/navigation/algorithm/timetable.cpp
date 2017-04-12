#include "navigation/algorithm/timetable.hpp"
#include "navigation/leg.hpp"

#include "id/line.hpp"
#include "id/stop.hpp"
#include "id/trip.hpp"

#include <algorithm>
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

boost::optional<Trip> TimeTable::
operator()(date::Time const departure, StopID const origin, StopID const destination) const
{
    struct State
    {
        StopID stop_id;
        date::Time arrival;
    };

    struct ReachedState
    {
        date::Time arrival;
        StopID parent;
        date::Time parent_departure;
        LineID line_id;
    };

    std::unordered_map<StopID, ReachedState> earliest_arrival;

    std::queue<State> que;

    // add all stops of the initial station into the que
    for (auto start : time_table.stops(time_table.station(origin)))
    {
        que.push({start, departure});
        // std::cout << "Adding: " << start << " " << departure << std::endl;
        earliest_arrival[start] = {departure, start, departure, WALKING_TRANSFER};
    }

    date::Time upper_bound;
    StopID reached_destination = destination;
    upper_bound = upper_bound + std::numeric_limits<std::uint32_t>::max();
    // relax by lines, in order of hops
    while (!que.empty())
    {
        // relax all hops
        auto reachable_this_hop = que.size();
        std::vector<State> this_round;
        for (std::size_t s = 0; s < reachable_this_hop; ++s)
        {
            auto state = que.front();
            que.pop();
            this_round.push_back(state);
        }

        std::sort(this_round.begin(), this_round.end(), [](auto const &lhs, auto const &rhs) {
            return lhs.arrival < rhs.arrival;
        });

        auto const destination_station = time_table.station(destination);
        auto const add_if_improved = [&](StopID const stop,
                                         date::Time const time,
                                         StopID const parent,
                                         LineID const line,
                                         date::Time const departure) {
            if (!earliest_arrival.count(stop) || time < earliest_arrival[stop].arrival)
            {
                if (time_table.station(stop) == destination_station && time < upper_bound)
                {
                    upper_bound = time;
                    reached_destination = stop;
                }
                BOOST_ASSERT(parent != stop);
                earliest_arrival[stop] = {time, parent, departure, line};
                // std::cout << "\t\t[Reach] " << stop << " from " << parent << " at " << time
                // << " via " << line << std::endl;
                return true;
            }
            else
                return false;
        };

        // std::cout << "[Round]" << std::endl;
        for (auto state : this_round)
        {
            // std::cout << "\tAt: " << state.stop_id << " " << state.arrival << std::endl;
            // stop when the earliest arrival is known
            if (upper_bound <= state.arrival)
                continue;

            // don't consider elements already looked at
            if (earliest_arrival[state.stop_id].arrival < state.arrival)
                continue;

            // get all lines at the given stop
            auto trip_range = stop_to_line(state.stop_id);
            auto const relax_line = [&](auto const line_id) {
                auto const trip_optional =
                    time_table.line(line_id).get(state.stop_id, state.arrival);
                if (!trip_optional)
                    return;

                auto const &trip = *trip_optional;
                auto time = trip.departure;

                auto duration_itr = trip.duration_range.begin();
                for (auto stop_itr = trip.stop_range.begin(); stop_itr != trip.stop_range.end();
                     ++stop_itr, ++duration_itr)
                {
                    auto const stop_id = *stop_itr;
                    if (add_if_improved(stop_id, time, state.stop_id, line_id, trip.departure))
                    {
                        auto transfers = time_table.transfers(stop_id);
                        // add all transfers
                        for (auto transfer : transfers)
                        {
                            auto transfer_time = time + std::max<int>(transfer.duration, 60);
                            if (add_if_improved(transfer.stop_id,
                                                transfer_time,
                                                stop_id,
                                                WALKING_TRANSFER,
                                                time) ||
                                (transfer.stop_id == stop_id))
                            {
                                // needs to be a transfer line instead of 0
                                que.push({transfer.stop_id, time + transfer.duration});
                            }
                        }
                    }
                    time = time + *duration_itr;
                }
            };

            for (auto line : trip_range)
                relax_line(line);
        }
    }

    if (!earliest_arrival.count(reached_destination))
        return boost::none;

    std::vector<PathEntry> path;
    auto current_stop = reached_destination;
    path.push_back({reached_destination,
                    earliest_arrival[reached_destination].line_id,
                    earliest_arrival[reached_destination].arrival,
                    earliest_arrival[reached_destination].parent_departure});
    do
    {
        current_stop = earliest_arrival[current_stop].parent;
        path.push_back({current_stop,
                        earliest_arrival[current_stop].line_id,
                        earliest_arrival[current_stop].arrival,
                        earliest_arrival[current_stop].parent_departure});
    } while (earliest_arrival[current_stop].parent != current_stop);

    std::reverse(path.begin(), path.end());

    return make_trip(path);
}

} // namespace algorithm
} // namespace navigation
} // namespace transit
