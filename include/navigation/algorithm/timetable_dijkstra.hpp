#ifndef TRANSIT_NAVIGATION_ALGORITHMS_TIMETABLE_DIJKSTRA_HPP_
#define TRANSIT_NAVIGATION_ALGORITHMS_TIMETABLE_DIJKSTRA_HPP_

#include "navigation/routing_algorithm.hpp"

#include "date/time.hpp"
#include "id/stop.hpp"
#include "id/trip.hpp"
#include "tool/container/kary_heap.hpp"

#include <boost/optional.hpp>

namespace nepomuk
{

namespace timetable
{
class TimeTable;
class TripTable;
class StopToTrip;
}

namespace navigation
{

class Route;

namespace algorithm
{

class TimeTableDijkstra : public RoutingAlgorithm
{
  public:
    using Base = RoutingAlgorithm;
    TimeTableDijkstra(timetable::TimeTable const &time_table,
                      timetable::StopToTrip const &stop_to_trip);

    // query a route between two stops
    boost::optional<Route> operator()(date::Time const departure,
                                      StopID const origin,
                                      StopID const destination) const override final;

    boost::optional<Route> operator()(date::Time const departure,
                                      std::vector<ADLeg> const &origins,
                                      std::vector<ADLeg> const &destinations) const override final;

  private:
    timetable::TripTable const &trip_table;
    timetable::StopToTrip const &stop_to_trip;

    struct ReachedVia
    {
        StopID parent_stop;
        TripID on_line;
        date::Time parent_departure;
    };
    using FourHeap = tool::container::KAryHeap<StopID, date::Time, 4, ReachedVia>;

    void relax_one(FourHeap &heap) const;

    std::vector<Base::PathEntry> extract_path(StopID current_stop, FourHeap const &heap) const;
};

} // namespace algorithm
} // namespace navigation
} // namespace nepomuk

#endif // TRANSIT_NAVIGATION_ALGORITHMS_TIMETABLE_DIJKSTRA_HPP_
