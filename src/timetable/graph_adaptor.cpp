#include "timetable/graph_adaptor.hpp"
#include "tool/container/forward_star_graph_factory.hpp"

#include "gtfs/stop.hpp"
#include "gtfs/time.hpp"

#include "tool/status/progress.hpp"

#include <boost/assert.hpp>
#include <iterator>
#include <set>

namespace
{

template <typename on_transfers_functor, typename on_stops_functor>
void do_stuff_on_graph(transit::timetable::TimeTable const &timetable,
                       transit::search::StopToLine const &stop_to_line,
                       on_transfers_functor on_transfers,
                       on_stops_functor on_stops)
{
    for (std::size_t stop = 0; stop < timetable.num_stops(); ++stop)
    {
        auto const stop_id = transit::gtfs::StopID{static_cast<std::uint64_t>(stop)};

        auto const transfers = timetable.transfers(stop_id);
        on_transfers(transfers);

        auto const lines = stop_to_line(stop_id);
        for (auto const &line : lines)
        {
            on_stops(timetable.line(line).stops().list(stop_id));
        }
    }
}

} // namespace

namespace transit
{
namespace timetable
{

tool::container::AdjacencyGraph
TimetableToGraphAdaptor::adapt(TimeTable const &timetable,
                               search::StopToLine const &stop_to_line)
{
    tool::status::FunctionTimingGuard guard("Connectivity Graph Creation");
    tool::container::ForwardStarGraphFactory factory;

    // calculate the number of edges
    std::size_t num_edges = 0;
    auto const count_transfers = [&num_edges](auto const transfer_range) {
        num_edges += std::distance(transfer_range.begin(), transfer_range.end());
    };
    auto const count_stops = [&num_edges,&timetable](auto const stop_range) {
        if (std::distance(stop_range.begin(), stop_range.end()) > 1)
            ++num_edges;

        if( timetable.station(stop_range.front()) != stop_range.front() )
            ++num_edges;

        auto direct = timetable.stops(timetable.station(*stop_range.begin())).size();
        // don't add self-loops
        num_edges += direct - 1;
    };

    do_stuff_on_graph(timetable, stop_to_line, count_transfers, count_stops);

    auto graph = factory.allocate(stop_to_line.size(), num_edges);

    // also adds the new node, since it is called on each stop
    auto const add_transfers = [&graph, &factory](auto const transfer_range) {
        factory.add_node(graph);

        for (auto transfer : transfer_range)
        {
            factory.add_edge(graph, transfer.stop_id.base());
        }
    };

    auto const add_next_stop = [&graph, &factory,&timetable](auto const stop_range) {
        if (std::distance(stop_range.begin(), stop_range.end()) > 1)
            factory.add_edge(graph, (stop_range.begin() + 1)->base());
        auto const me = *stop_range.begin();

        if( timetable.station(stop_range.front()) != stop_range.front() )
            factory.add_edge(graph, timetable.station(stop_range.front()).base());

        auto const &all = timetable.stops(timetable.station(stop_range.front()));
        for (auto other : all)
        {
            if (other == me)
                continue;
            factory.add_edge(graph, other.base());
        }
    };

    do_stuff_on_graph(timetable, stop_to_line, add_transfers, add_next_stop);

    for( auto const& node : graph.nodes() )
    {
        std::set<std::size_t> targets;
        for( auto const& edge : graph.edges(node) )
            targets.insert(graph.offset(edge.target()));
    }

    BOOST_ASSERT_MSG(factory.valid(),
                     "This is seriously messed up. The timetable graph adaptor seems broken.");

    return graph;
}

} // namespace timetable
} // namespace transit
