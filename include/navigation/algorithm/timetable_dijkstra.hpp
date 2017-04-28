#ifndef TRANSIT_NAVIGATION_ALGORITHMS_TIMETABLE_DIJKSTRA_HPP_
#define TRANSIT_NAVIGATION_ALGORITHMS_TIMETABLE_DIJKSTRA_HPP_

#include "navigation/routing_algorithm.hpp"
#include "search/stop_to_line.hpp"
#include "timetable/timetable.hpp"

#include "date/time.hpp"
#include "id/stop.hpp"
#include "tool/container/kary_heap.hpp"

#include <boost/optional.hpp>

namespace transit
{
namespace navigation
{
namespace algorithm
{

class TimeTableDijkstra : public RoutingAlgorithm
{
  public:
    using Base = RoutingAlgorithm;
    TimeTableDijkstra(timetable::TimeTable const &time_table,
                      search::StopToLine const &stop_to_line);

    // query a route between two stops
    boost::optional<Route> operator()(date::Time const departure,
                                      StopID const origin,
                                      StopID const destination) const override final;

    boost::optional<Route> operator()(date::Time const departure,
                                      std::vector<ADLeg> const &origins,
                                      std::vector<ADLeg> const &destinations) const override final;

  private:
    struct ReachedVia
    {
        StopID parent_stop;
        LineID on_line;
        date::Time parent_departure;
    };
    using FourHeap = tool::container::KAryHeap<StopID, date::Time, 4, ReachedVia>;

    // the unmodified timetable data to route on
    timetable::TimeTable const &time_table;

    // the look-up for lines from a given stop
    search::StopToLine const &stop_to_line;

    void relax_one(FourHeap &heap) const;

    std::vector<Base::PathEntry> extract_path(StopID current_stop, FourHeap const &heap) const;
};

} // namespace algorithm
} // namespace navigation
} // namespace transit

#endif // TRANSIT_NAVIGATION_ALGORITHMS_TIMETABLE_DIJKSTRA_HPP_
