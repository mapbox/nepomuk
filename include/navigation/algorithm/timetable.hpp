#ifndef NEPOMUK_NAVIGATION_ALGORITHMS_TIMETABLE_HPP_
#define NEPOMUK_NAVIGATION_ALGORITHMS_TIMETABLE_HPP_

#include "navigation/routing_algorithm.hpp"

#include "date/time.hpp"
#include "id/line.hpp"
#include "id/stop.hpp"

#include <boost/optional.hpp>
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace nepomuk
{
namespace timetable
{
class StopToTrip;
} // namespace timetable
namespace navigation
{
class Route;
namespace algorithm
{

class TimeTable : public RoutingAlgorithm
{
  public:
    TimeTable(timetable::TimeTable const &time_table, timetable::StopToTrip const &stop_to_trip);

    // query a route between two stops
    boost::optional<Route> operator()(date::Time const departure,
                                      StopID const origin,
                                      StopID const destination) const override final;

    boost::optional<Route> operator()(date::Time const departure,
                                      std::vector<ADLeg> const &origins,
                                      std::vector<ADLeg> const &destinations) const override final;

  private:
    // internal structures
    struct State
    {
        StopID stop_id;
        date::Time arrival;
    };

    struct ReachedState
    {
        date::Time arrival;
        StopID parent;
        date::Time parent_departure;
        TripID line_id;
    };

    using StateContainer = std::vector<State>;
    using ReachedStateContainer = std::unordered_map<StopID, ReachedState>;

    void add_origin(StopID const origin,
                    date::Time const departure,
                    StateContainer &state,
                    ReachedStateContainer &reached) const;

    void relax(date::Time const &upper_bound,
               std::size_t offset,
               std::size_t count,
               StateContainer &state,
               ReachedStateContainer &earliest_arrival) const;

    std::vector<PathEntry> extract_path(StopID last_stop,
                                        ReachedStateContainer const &container) const;

    timetable::StopToTrip const &stop_to_trip;
};

} // namespace algorithm
} // namespace navigation
} // namespace nepomuk

#endif // NEPOMUK_NAVIGATION_ALGORITHMS_TIMETABLE_HPP_
