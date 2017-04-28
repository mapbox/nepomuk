#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "navigation/leg.hpp"

#include "tool/container/kary_heap.hpp"

#include "id/stop.hpp"

#include <iterator>
#include <set>
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

boost::optional<Route> TimeTableDijkstra::
operator()(date::Time const departure, StopID const origin, StopID const destination) const
{
    FourHeap heap;

    auto const destination_station = time_table.station(destination);

    StopID reached_destination = destination;

    // add all stops of the starting station to the heap
    for (auto start : time_table.stops(time_table.station(origin)))
    {
        // std::cout << "[start] " << start << " " << departure << std::endl;
        heap.insert(start, departure, {start, WALKING_TRANSFER, departure});
    }

    // relax by lines, in order of hops
    while (!heap.empty())
    {
        // found destination
        auto const stop = heap.min();
        // std::cout << "[at] " << stop << " " << heap.min_key() << std::endl;

        if (time_table.station(stop) == destination_station)
            reached_destination = stop;

        relax_one(heap);
    }

    if (!heap.reached(destination))
        return boost::none;

    return make_route(extract_path(reached_destination, heap));
}

boost::optional<Route> TimeTableDijkstra::operator()(date::Time const departure,
                                                     std::vector<ADLeg> const &origins,
                                                     std::vector<ADLeg> const &destinations) const
{
    if (origins.empty() || destinations.empty())
        return boost::none;

    FourHeap heap;
    StopID destination = destinations.front().stop;

    auto const add_stops = [&](auto const stop, auto const offset) {
        // add all stops of the starting station to the heap
        auto const start_at_station = departure + offset;
        for (auto start : time_table.stops(time_table.station(stop)))
        {
            // std::cout << "[start] " << start << " " << start_at_station << std::endl;
            if (!heap.reached(start))
                heap.insert(start, start_at_station, {start, WALKING_TRANSFER, departure});
            else if ((departure + offset) < heap.key(start))
                heap.decrease_key(start, start_at_station, {start, WALKING_TRANSFER, departure});
        }
    };

    for (auto const &origin : origins)
        add_stops(origin.stop, origin.seconds);

    date::Time best = date::Time::infinity();

    auto const check_and_update_best = [&](StopID const stop, date::Time const arrival) {
        auto const is_destination = [stop](auto const &leg) { return leg.stop == stop; };
        auto const itr = std::find_if(destinations.begin(), destinations.end(), is_destination);
        if (itr != destinations.end() && itr->seconds + arrival < best)
        {
            best = itr->seconds + arrival;
            destination = itr->stop;
        }
    };

    std::set<StopID> destination_stations;
    std::transform(origins.begin(),
                   origins.end(),
                   std::inserter(destination_stations, destination_stations.end()),
                   [&](auto const &leg) { return time_table.station(leg.stop); });
    std::transform(destinations.begin(),
                   destinations.end(),
                   std::inserter(destination_stations, destination_stations.end()),
                   [&](auto const &leg) { return time_table.station(leg.stop); });

    while (!heap.empty())
    {
        // found destination
        auto const stop = heap.min();
        auto const arrival = heap.min_key();
        // std::cout << "[at] " << stop << " " << arrival << std::endl;

        check_and_update_best(stop, arrival);

        // reached the final destination
        if (best < arrival)
            break;

        relax_one(heap);
    }

    if (!heap.reached(destination))
        return boost::none;

    return make_route(extract_path(destination, heap));
}

void TimeTableDijkstra::relax_one(FourHeap &heap) const
{
    auto const reach = [&](StopID const stop,
                           date::Time const time,
                           StopID from_stop,
                           LineID on_line,
                           date::Time const parent_departure) {
        // std::cout << "  [reached] " << stop << " " << time << " Parent: " << from_stop << " "
        //          << parent_departure << " Line: " << on_line << std::endl;
        if (!heap.reached(stop))
        {
            heap.insert(stop, time, {from_stop, on_line, parent_departure});
            return true;
        }
        else if (time < heap.key(stop))
        {
            heap.decrease_key(stop, time, {from_stop, on_line, parent_departure});
            return true;
        }
        else
            return false;
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

            if (reach(stop_id, time, from_stop_id, from_line_id, trip.departure))
            {
                // add all transfers
                for (auto transfer : transfers)
                {
                    auto transfer_time = time + transfer.duration;
                    // std::cout << "  [transfer]";
                    reach(transfer.stop_id, transfer_time, stop_id, WALKING_TRANSFER, time);
                }

                // reach all stops at the same station
                auto station_time = time + 120;
                for (auto neighbor : time_table.stops(time_table.station(stop_id)))
                {
                    if (neighbor != stop_id)
                    {
                        // std::cout << " [station]";
                        reach(neighbor, station_time, stop_id, WALKING_TRANSFER, time);
                    }
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

    // found destination
    auto const stop = heap.min();
    auto const arrival = heap.min_key();
    heap.delete_min();

    auto const lines = stop_to_line(stop);
    for (auto line_id : lines)
    {
        traverse_line(line_id, stop, arrival);
    }
}

std::vector<TimeTableDijkstra::Base::PathEntry>
TimeTableDijkstra::extract_path(StopID current_stop, FourHeap const &heap) const
{
    std::vector<Base::PathEntry> path;
    path.push_back({current_stop,
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
    return path;
}

} // namespace algorithm
} // namespace navigation
} // namespace transit
