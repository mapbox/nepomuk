#include "timetable/graph_adaptor.hpp"
#include "tool/container/forward_star_graph_factory.hpp"

#include "gtfs/stop.hpp"
#include "gtfs/time.hpp"

#include "tool/status/progress.hpp"

#include <iterator>

namespace
{

template <typename on_transfers_functor, typename on_stops_functor>
void do_stuff_on_graph(transit::timetable::TimeTable const &timetable,
                       transit::search::StopToLine const &stop_to_line,
                       on_transfers_functor on_transfers,
                       on_stops_functor on_stops)
{
    for (std::size_t stop = 0; stop < stop_to_line.size(); ++stop)
    {
        auto const stop_id = transit::gtfs::StopID{static_cast<std::uint64_t>(stop)};
        auto const lines = stop_to_line(stop_id);
        auto const transfers = timetable.list_transfers(stop_id);
        on_transfers(transfers);
        for (auto const &line : lines)
        {
            auto const trip_optional = timetable.list_trips(line, transit::gtfs::Time());
            if (trip_optional)
            {
                auto const &trip = *trip_optional;
                auto const stops = trip.stop_table.list(stop_id);
                on_stops(stops);
            }
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
    auto const count_stops = [&num_edges](auto const stop_range) {
        if (std::distance(stop_range.begin(), stop_range.end()) > 1)
            ++num_edges;
    };

    do_stuff_on_graph(timetable, stop_to_line, count_transfers, count_stops);

    auto graph = factory.allocate(stop_to_line.size(), num_edges);

    // also adds the new node, since it is called on each stop
    auto const add_transfers = [&graph, &factory](auto const transfer_range) {
        factory.add_node(graph);
        for (auto transfer : transfer_range)
            factory.add_edge(graph, static_cast<std::uint64_t>(transfer.stop_id));
    };

    auto const add_next_stop = [&graph, &factory](auto const stop_range) {
        if (std::distance(stop_range.begin(), stop_range.end()) > 1)
            factory.add_edge(graph, static_cast<std::uint64_t>(*(stop_range.begin() + 1)));
    };

    do_stuff_on_graph(timetable, stop_to_line, add_transfers, add_next_stop);

    return graph;
}

} // namespace timetable
} // namespace transit
