#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "navigation/leg.hpp"

#include "tool/container/kary_heap.hpp"

#include "id/stop.hpp"

#include <utility>

namespace transit
{
namespace navigation
{
namespace algorithm
{

TimeTableDijkstra::TimeTableDijkstra(timetable::TimeTable const &time_table,
                                     search::StopToLine const &stop_to_line)
    : RoutingAlgorithm(time_table.lines()), time_table(time_table), stop_to_line(stop_to_line)
{
}

boost::optional<Trip> TimeTableDijkstra::
operator()(date::Time const departure, StopID const origin, StopID const destination) const
{
    struct ReachedVia
    {
        StopID parent_stop;
        LineID on_line;
        date::Time parent_departure;
    };
    using FourHeap = tool::container::KAryHeap<StopID, date::Time, 4, ReachedVia>;
    FourHeap heap;

    auto const destination_station = time_table.station(destination);

    auto const reach = [&](StopID const stop,
                           date::Time const time,
                           StopID from_stop,
                           LineID on_line,
                           bool use_in_heap,
                           date::Time const parent_departure) {

        if (use_in_heap)
        {
            if (!heap.reached(stop))
            {
                heap.insert(stop, time, {from_stop, on_line, parent_departure});
                return true;
            }
            else if (time < heap.key(stop))
            {
                if (heap.contains(stop))
                    heap.decrease_key(stop, time, {from_stop, on_line, parent_departure});
                else
                    heap.insert(stop, time, {from_stop, on_line, parent_departure});

                return true;
            }
            else
                return false;
        }
        else
        {
            if (!heap.reached(stop) || time < heap.key(stop))
            {
                heap.reach_non_inserting(stop, time, {from_stop, on_line, parent_departure});
                return true;
            }
            else
            {
                return false;
            }
        }
    };

    auto const run_trip = [&](auto const from_stop_id, auto const from_line_id, auto const &trip) {
        auto time = trip.departure;
        if (std::distance(trip.stop_range.begin(), trip.stop_range.end()) == 0)
            return;

        // iterator over all trips
        auto duration_itr = trip.duration_range.begin();
        for (auto stop_itr = trip.stop_range.begin(); stop_itr != trip.stop_range.end();
             ++stop_itr, ++duration_itr)
        {
            auto const stop_id = *stop_itr;
            auto transfers = time_table.transfers(stop_id);
            auto const is_transfer_station =
                std::find_if(transfers.begin(), transfers.end(), [stop_id](auto const &transfer) {
                    return transfer.stop_id == stop_id;
                }) != transfers.end();

            if (reach(stop_id,
                      time,
                      from_stop_id,
                      from_line_id,
                      is_transfer_station || (destination_station == time_table.station(stop_id)),
                      trip.departure))
            {
                // add all transfers
                for (auto transfer : transfers)
                {
                    auto transfer_time = time + transfer.duration;
                    // if (transfer.stop_id != stop_id)
                    reach(transfer.stop_id, transfer_time, stop_id, WALKING_TRANSFER, true, time);
                }
            }
            time = time + *duration_itr;
        }
    };

    auto const traverse_line = [&](auto const line_id, auto const stop_id, auto const time) {
        auto const trip = time_table.line(line_id).get(stop_id, time);
        if (trip)
            run_trip(stop_id, line_id, *trip);
    };

    // add all stops of the starting station to the heap
    for (auto start : time_table.stops(time_table.station(origin)))
    {
        heap.insert(start, departure, {start, WALKING_TRANSFER, departure});
    }

    StopID reached_destination = destination;

    // relax by lines, in order of hops
    while (!heap.empty())
    {
        // found destination
        auto const stop = heap.min();
        // reached the station
        if (time_table.station(stop) == destination_station)
        {
            reached_destination = stop;
            break;
        }

        auto const arrival = heap.min_key();
        heap.delete_min();

        auto const lines = stop_to_line(stop);
        for (auto line_id : lines)
        {
            traverse_line(line_id, stop, arrival);
        }
    }

    if (!heap.reached(destination))
        return boost::none;

    std::vector<Base::PathEntry> path;
    auto current_stop = destination;
    path.push_back({destination,
                    heap.data(current_stop).on_line,
                    heap.key(current_stop),
                    heap.data(current_stop).parent_departure});
    do
    {
        current_stop = heap.data(current_stop).parent_stop;
        path.push_back({current_stop,
                        heap.data(current_stop).on_line,
                        heap.key(current_stop),
                        heap.data(current_stop).parent_departure});
    } while (heap.data(current_stop).parent_stop != current_stop);

    std::reverse(path.begin(), path.end());

    return make_trip(path);
}

} // namespace algorithm
} // namespace navigation
} // namespace transit
