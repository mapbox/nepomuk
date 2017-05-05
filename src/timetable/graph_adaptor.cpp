#include "timetable/graph_adaptor.hpp"
#include "id/stop.hpp"
#include "search/stop_to_line.hpp"  // for StopToLine
#include "timetable/line_table.hpp" // for LineTable
#include "timetable/stop_table.hpp" // for StopTable
#include "timetable/timetable.hpp"  // for TimeTable
#include "tool/container/forward_star_graph_factory.hpp"
#include "tool/status/progress.hpp"

#include <boost/assert.hpp>
#include <cstddef>
#include <iterator>
#include <set>

namespace
{

template <typename on_transfers_functor, typename on_stops_functor, typename on_lines_functor>
void do_stuff_on_graph(transit::timetable::TimeTable const &timetable,
                       transit::search::StopToLine const &stop_to_line,
                       on_transfers_functor on_transfers,
                       on_stops_functor on_stops,
                       on_lines_functor on_lines)
{
    for (std::size_t stop = 0; stop < timetable.num_stops(); ++stop)
    {
        auto const stop_id = transit::StopID{static_cast<std::uint64_t>(stop)};

        auto const transfers = timetable.transfers(stop_id);
        on_transfers(transfers);

        on_stops(stop_id);

        auto const lines = stop_to_line(stop_id);
        for (auto const &line : lines)
        {
            on_lines(timetable.line(line).stops().list(stop_id));
        }
    }
}

} // namespace

namespace transit
{
namespace timetable
{

tool::container::AdjacencyGraph
TimetableToGraphAdaptor::adapt(TimeTable const &timetable, search::StopToLine const &stop_to_line)
{
    tool::status::FunctionTimingGuard guard("Connectivity Graph Creation");
    tool::container::ForwardStarGraphFactory factory;

    // calculate the number of edges
    std::size_t num_edges = 0;
    auto const count_transfers = [&num_edges](auto const transfer_range) {
        num_edges += std::distance(transfer_range.begin(), transfer_range.end());
    };
    auto const count_connections = [&num_edges, &timetable](auto const stop_range) {
        if (std::distance(stop_range.begin(), stop_range.end()) > 1)
            ++num_edges;
    };
    auto const count_direct_connections = [&num_edges, &timetable](auto const stop_id) {
        if (timetable.station(stop_id) != stop_id)
            ++num_edges;

        auto direct = timetable.stops(timetable.station(stop_id)).size();
        // don't add self-loops
        num_edges += direct - 1;
    };

    do_stuff_on_graph(
        timetable, stop_to_line, count_transfers, count_direct_connections, count_connections);

    auto graph = factory.allocate(stop_to_line.size(), num_edges);

    // also adds the new node, since it is called on each stop
    auto const add_transfers = [&graph, &factory](auto const transfer_range) {
        factory.add_node(graph);

        for (auto transfer : transfer_range)
        {
            factory.add_edge(graph, transfer.stop_id.base());
        }
    };

    auto const add_next_stop_on_line = [&graph, &factory, &timetable](auto const stop_range) {
        if (std::distance(stop_range.begin(), stop_range.end()) > 1)
            factory.add_edge(graph, (stop_range.begin() + 1)->base());
    };

    auto const add_station_connections = [&graph, &factory, &timetable](auto const stop_id) {
        if (timetable.station(stop_id) != stop_id)
            factory.add_edge(graph, timetable.station(stop_id).base());

        auto const &all = timetable.stops(timetable.station(stop_id));
        for (auto other : all)
        {
            // no self loop
            if (other == stop_id)
                continue;
            factory.add_edge(graph, other.base());
        }
    };

    do_stuff_on_graph(
        timetable, stop_to_line, add_transfers, add_station_connections, add_next_stop_on_line);

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
} // namespace transit
