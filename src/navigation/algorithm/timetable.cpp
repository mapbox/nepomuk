#include "navigation/algorithm/timetable.hpp"
#include "navigation/leg.hpp"

#include "gtfs/stop.hpp"
#include "gtfs/time.hpp"
#include "gtfs/trip.hpp"

#include <algorithm>
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
    : time_table(time_table), stop_to_line(stop_to_line)
{
}

boost::optional<Trip> TimeTable::operator()(gtfs::Time const departure,
                                            gtfs::StopID const origin,
                                            gtfs::StopID const destination) const
{
    struct State
    {
        gtfs::StopID stop_id;
        gtfs::Time arrival;
    };

    struct ReachedState
    {
        gtfs::Time arrival;
        gtfs::StopID parent;
        timetable::LineID line_id;
    };

    std::unordered_map<gtfs::StopID, ReachedState> earliest_arrival;

    std::queue<State> que;
    que.push({origin, departure});
    earliest_arrival[origin] = {
        departure, origin, timetable::LineID{static_cast<std::uint64_t>(-1)}};

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

        for (auto state : this_round)
        {
            // stop when the earliest arrival is known
            if (earliest_arrival.count(destination) &&
                earliest_arrival[destination].arrival < state.arrival)
                continue;
            // get all lines at the given stop
            auto trip_range = stop_to_line(state.stop_id);
            auto const relax_line = [&](auto const line_id) {
                auto const trips = time_table.get(line_id, state.arrival);
                for (auto const &trip : trips)
                {
                    auto time = trip.departure;
                    auto const stop_range = trip.stop_table->list(state.stop_id);
                    auto const duration_range = trip.duration_table->list(
                        std::distance(trip.stop_table->list().begin(), stop_range.begin()));
                    auto duration_itr = duration_range.begin();
                    for (auto stop_itr = stop_range.begin(); stop_itr != stop_range.end();
                         ++stop_itr, ++duration_itr)
                    {
                        auto const stop_id = *stop_itr;
                        if (!earliest_arrival.count(stop_id) ||
                            time < earliest_arrival.find(stop_id)->second.arrival)
                        {
                            // reach node:
                            earliest_arrival[stop_id] = {time, state.stop_id, line_id};
                            auto inner_trip_range = stop_to_line(stop_id);
                            if (std::distance(inner_trip_range.begin(), inner_trip_range.end()) > 1)
                                que.push({stop_id, time});
                        }
                        time = time + *duration_itr;
                    }
                }
            };

            for (auto line : trip_range)
            {
                relax_line(line);
            }
        }
    }

    if (!earliest_arrival.count(destination))
        return boost::none;

    struct ReachedOnPath
    {
        gtfs::StopID stop_id;
        gtfs::Time arrival;
        timetable::LineID line_id;
    };
    std::vector<ReachedOnPath> path;
    auto current_stop = destination;
    path.push_back({destination,
                    earliest_arrival[destination].arrival,
                    earliest_arrival[destination].line_id});
    while (current_stop != origin)
    {
        current_stop = earliest_arrival[current_stop].parent;
        path.push_back({current_stop,
                        earliest_arrival[current_stop].arrival,
                        earliest_arrival[current_stop].line_id});
    }
    std::reverse(path.begin(), path.end());

    Trip result;
    Leg leg;
    if (path.size() > 1)
        path[0].line_id = path[1].line_id;

    auto current_line = path[0].line_id;
    for (auto itr = path.begin(); itr != path.end(); ++itr)
    {
        if (itr->line_id != current_line)
        {
            add_leg(result, std::move(leg));
            current_line = itr->line_id;
            leg = Leg();
            // add the location of the step again
            add_stop(leg, Leg::stop_type{(itr - 1)->stop_id, (itr - 1)->arrival});
        }
        add_stop(leg, Leg::stop_type{itr->stop_id, itr->arrival});
    }
    // add the final leg
    add_leg(result, std::move(leg));
    return result;
}

} // namespace algorithm
} // namespace navigation
} // namespace transit
