#include "navigation/algorithm/timetable.hpp"
#include "navigation/leg.hpp"

#include "gtfs/stop.hpp"
#include "gtfs/time.hpp"
#include "gtfs/trip.hpp"

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

    // add all stops of the initial station into the que
    for (auto start : time_table.stops(time_table.station(origin)))
    {
        que.push({start, departure});
        earliest_arrival[start] = {
            departure, start, timetable::LineID{static_cast<std::uint64_t>(-1)}};
    }

    gtfs::Time upper_bound;
    gtfs::StopID reached_destination = destination;
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
        auto const add_if_improved = [&](gtfs::StopID const stop,
                                         gtfs::Time const time,
                                         gtfs::StopID const parent,
                                         timetable::LineID const line) {
            if (!earliest_arrival.count(stop) || time < earliest_arrival[stop].arrival)
            {
                if (time_table.station(stop) == destination_station && time < upper_bound)
                {
                    upper_bound = time;
                    reached_destination = stop;
                }
                BOOST_ASSERT(parent != stop);
                earliest_arrival[stop] = {time, parent, line};
                return true;
            }
            else
                return false;
        };

        for (auto state : this_round)
        {
            // stop when the earliest arrival is known
            if (upper_bound <= state.arrival)
                continue;

            // don't consider elements already looked at
            if (earliest_arrival[state.stop_id].arrival < state.arrival)
                continue;

            // get all lines at the given stop
            auto trip_range = stop_to_line(state.stop_id);
            auto const relax_line = [&](auto const line_id) {
                auto const trip_optional = time_table.line(line_id).get(state.arrival);
                if (!trip_optional)
                    return;

                auto const &trip = *trip_optional;
                auto time = trip.departure;
                auto stop_range = [&]() {
                    // this part is necessary due to loops, see
                    // https://github.com/mapbox/directions-transit/issues/66
                    auto const stop_range = trip.stop_table.list(state.stop_id);
                    auto const duration_range = trip.duration_table.list(
                        std::distance(trip.stop_table.list().begin(), stop_range.begin()));
                    if (time + duration_range.front() < state.arrival)
                    {
                        auto itr =
                            std::find(stop_range.begin() + 1, stop_range.end(), state.stop_id);
                        return boost::make_iterator_range(itr, stop_range.end());
                    }
                    else
                    {
                        return stop_range;
                    }
                }();

                auto const duration_range = trip.duration_table.list(
                    std::distance(trip.stop_table.list().begin(), stop_range.begin()));

                auto duration_itr = duration_range.begin();
                for (auto stop_itr = stop_range.begin(); stop_itr != stop_range.end();
                     ++stop_itr, ++duration_itr)
                {
                    auto const stop_id = *stop_itr;
                    if (add_if_improved(stop_id, time, state.stop_id, line_id))
                    {
                        // reach node:
                        auto transfers = time_table.transfers(stop_id);
                        // add all transfers
                        for (auto transfer : transfers)
                        {
                            auto transfer_time = time + std::max<int>(transfer.duration, 60);
                            if (add_if_improved(transfer.stop_id, transfer_time, stop_id, {0}) ||
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

    struct ReachedOnPath
    {
        gtfs::StopID stop_id;
        gtfs::Time arrival;
        timetable::LineID line_id;
    };
    std::vector<ReachedOnPath> path;
    auto current_stop = reached_destination;
    path.push_back({reached_destination,
                    earliest_arrival[reached_destination].arrival,
                    earliest_arrival[reached_destination].line_id});
    do
    {
        current_stop = earliest_arrival[current_stop].parent;
        path.push_back({current_stop,
                        earliest_arrival[current_stop].arrival,
                        earliest_arrival[current_stop].line_id});
    } while (earliest_arrival[current_stop].parent != current_stop);

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
            set_departure(leg, itr->arrival);
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
