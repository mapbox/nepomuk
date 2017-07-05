#include "timetable/graph_adaptor.hpp"
#include "id/stop.hpp"
#include "timetable/stop_to_trip.hpp"
#include "timetable/timetable.hpp" // for TimeTable
#include "tool/container/forward_star_graph_factory.hpp"
#include "tool/status/progress.hpp"

#include <boost/assert.hpp>
#include <cstddef>
#include <iterator>
#include <set>

namespace
{

template <typename on_transfers_functor, typename on_stops_functor, typename on_lines_functor>
void do_stuff_on_graph(nepomuk::timetable::TimeTable const &timetable,
                       nepomuk::timetable::StopToTrip const &stop_to_trip,
                       on_transfers_functor on_transfers,
                       on_stops_functor on_stops,
                       on_lines_functor on_lines)
{
    auto const &trip_table = timetable.trip_table();
    for (std::size_t stop = 0; stop < timetable.num_stops(); ++stop)
    {
        auto const stop_id = nepomuk::StopID{static_cast<std::uint64_t>(stop)};

        auto const transfers = timetable.transfers(stop_id);
        on_transfers(transfers);

        on_stops(stop_id);

        auto const lines = stop_to_trip(stop_id);
        for (auto const &line : lines)
        {
            auto trip_itr = trip_table(line.trip_id, line.offset);
            on_lines(trip_itr);
        }
    }
}

} // namespace

namespace nepomuk
{
namespace timetable
{

tool::container::AdjacencyGraph
TimetableToGraphAdaptor::adapt(TimeTable const &timetable,
                               timetable::StopToTrip const &stop_to_trip)
{
    tool::status::FunctionTimingGuard guard("Connectivity Graph Creation");
    tool::container::ForwardStarGraphFactory factory;

    // calculate the number of edges
    std::size_t num_edges = 0;

    auto const count_transfers = [&num_edges](auto const transfer_range) {
        num_edges += std::distance(transfer_range.begin(), transfer_range.end());
    };
    std::set<std::pair<StopID, StopID>> seen_edges;
    auto const count_connections = [&num_edges, &timetable, &seen_edges](auto trip_itr) {
        if (!trip_itr.valid())
            return;

        if (!trip_itr.has_next())
            return;

        auto const from = trip_itr.stop();
        ++trip_itr;
        auto const to = trip_itr.stop();
        auto pair = std::make_pair(from, to);
        if (!seen_edges.count(pair))
        {
            seen_edges.insert(pair);
            ++num_edges;
        }
    };
    auto const count_direct_connections = [&num_edges, &timetable](auto const stop_id) {
        if (timetable.station(stop_id) != stop_id)
            ++num_edges;
        else
        {
            auto direct = timetable.stops(timetable.station(stop_id)).size();
            // don't add self-loops
            num_edges += direct - 1;
        }
    };

    do_stuff_on_graph(
        timetable, stop_to_trip, count_transfers, count_direct_connections, count_connections);

    auto graph = factory.allocate(timetable.num_stops(), num_edges);

    // also adds the new node, since it is called on each stop
    auto const add_transfers = [&graph, &factory](auto const transfer_range) {
        factory.add_node(graph);

        for (auto transfer : transfer_range)
        {
            factory.add_edge(graph, transfer.stop_id.base());
        }
    };

    auto const add_next_stop_on_line = [&graph, &factory, &timetable, &seen_edges](auto trip_itr) {
        if (!trip_itr.valid())
            return;

        if (!trip_itr.has_next())
            return;

        auto const from = trip_itr.stop();
        ++trip_itr;
        auto const to = trip_itr.stop();
        auto pair = std::make_pair(from, to);

        if (!seen_edges.count(pair))
        {
            seen_edges.insert(pair);
            factory.add_edge(graph, pair.second.base());
        }
    };

    auto const add_station_connections = [&graph, &factory, &timetable](auto const stop_id) {
        // add the connection to the station
        if (timetable.station(stop_id) != stop_id)
            factory.add_edge(graph, timetable.station(stop_id).base());

        else
        {
            // add the connection from the station to all other stops
            auto const &all = timetable.stops(timetable.station(stop_id));
            for (auto other : all)
            {
                // no self loop
                if (other == stop_id)
                    continue;
                factory.add_edge(graph, other.base());
            }
        }
    };

    seen_edges.clear();
    do_stuff_on_graph(
        timetable, stop_to_trip, add_transfers, add_station_connections, add_next_stop_on_line);

    for (auto const &node : graph.nodes())
    {
        std::set<std::size_t> targets;
        for (auto const &edge : graph.edges(node))
            targets.insert(graph.offset(edge.target()));
    }

    BOOST_ASSERT_MSG(factory.valid(),
                     "This is seriously messed up. The timetable graph adaptor seems broken.");

    return graph;
}

} // namespace timetable
} // namespace nepomuk
