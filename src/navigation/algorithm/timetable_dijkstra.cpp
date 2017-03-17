#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "navigation/leg.hpp"

#include "tool/container/kary_heap.hpp"

#include "gtfs/stop.hpp"
#include "gtfs/time.hpp"
#include "gtfs/trip.hpp"

#include <utility>

namespace transit
{
namespace navigation
{
namespace algorithm
{

TimeTableDijkstra::TimeTableDijkstra(timetable::TimeTable const &time_table,
                                     search::StopToLine const &stop_to_line)
    : time_table(time_table), stop_to_line(stop_to_line)
{
}

boost::optional<Trip> TimeTableDijkstra::operator()(gtfs::Time const departure,
                                                    gtfs::StopID const origin,
                                                    gtfs::StopID const destination) const
{
    using FourHeap = tool::container::
        KAryHeap<gtfs::StopID, gtfs::Time, 4, std::pair<gtfs::StopID, timetable::LineID>>;
    FourHeap heap;

    auto const destination_station = time_table.station(destination);
    // std::cout << "Running to station: " << destination_station << "(Stops:";
    // for (auto stop : time_table.stops(destination_station))
    // std::cout << " " << stop;
    // std::cout << std::endl;

    auto const reach = [&](gtfs::StopID const stop,
                           gtfs::Time const time,
                           gtfs::StopID from_stop,
                           timetable::LineID on_line,
                           bool use_in_heap) {
        if (use_in_heap)
        {
            if (!heap.reached(stop))
            {
                // std::cout << "\tnew: " << stop << " " << time << std::endl;
                heap.insert(stop, time, std::make_pair(from_stop, on_line));
                return true;
            }
            else if (time < heap.key(stop))
            {
                // std::cout << "\timprove: " << stop << " " << time << std::endl;
                if (heap.contains(stop))
                    heap.decrease_key(stop, time, std::make_pair(from_stop, on_line));
                else
                    heap.insert(stop, time, std::make_pair(from_stop, on_line));

                return true;
            }
            else if (time == heap.key(stop) && !heap.contains(stop))
            {
                // std::cout << "\ttransfer: " << stop << " " << time << std::endl;
                heap.insert(stop, time, std::make_pair(from_stop, on_line));
                return false;
            }
            else
                return false;
        }
        else
        {
            if (!heap.reached(stop) || time < heap.key(stop))
            {
                // std::cout << "\treach: " << stop << " " << time << std::endl;
                heap.reach_non_inserting(stop, time, std::make_pair(from_stop, on_line));
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
        auto const stop_range = trip.stop_table.list(from_stop_id);

        if (std::distance(stop_range.begin(), stop_range.end()) == 0)
            return;

        auto const duration_range = trip.duration_table.list(
            std::distance(trip.stop_table.list().begin(), stop_range.begin()));

        // iterator over all trips
        auto duration_itr = duration_range.begin();
        for (auto stop_itr = stop_range.begin(); stop_itr != stop_range.end();
             ++stop_itr, ++duration_itr)
        {
            auto const stop_id = *stop_itr;
            auto transfers = time_table.list_transfers(stop_id);
            if (reach(stop_id,
                      time,
                      from_stop_id,
                      from_line_id,
                      destination_station == time_table.station(stop_id)))
            {
                // add all transfers
                for (auto transfer : transfers)
                {
                    auto transfer_time = time + transfer.duration;
                    if (transfer.stop_id != stop_id)
                        reach(transfer.stop_id, transfer_time, stop_id, {0}, true);
                }
            }
            time = time + *duration_itr;
        }
    };

    auto const traverse_line = [&](auto const line_id, auto const stop_id, auto const time) {
        auto const trip = time_table.list_trips(line_id, time);
        if (trip)
            run_trip(stop_id, line_id, *trip);
    };

    // add all stops of the starting station to the heap
    for (auto start : time_table.stops(time_table.station(origin)))
        heap.insert(start, departure, std::make_pair(start, timetable::LineID{0}));

    gtfs::StopID reached_destination = destination;

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
        // std::cout << "at: " << stop << " with " << arrival << std::endl;
        heap.delete_min();

        auto const lines = stop_to_line(stop);
        for (auto line_id : lines)
            traverse_line(line_id, stop, arrival);
    }

    if (!heap.reached(destination))
        return boost::none;

    struct ReachedOnPath
    {
        gtfs::StopID stop_id;
        gtfs::Time arrival;
        timetable::LineID line_id;
    };

    std::vector<ReachedOnPath> path;
    auto current_stop = destination;
    path.push_back({destination, heap.key(current_stop), heap.data(current_stop).second});
    do
    {
        current_stop = heap.data(current_stop).first;
        path.push_back({current_stop, heap.key(current_stop), heap.data(current_stop).second});
    } while (heap.data(current_stop).first != current_stop);

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
