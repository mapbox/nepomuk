#ifndef TRANSIT_NAVIGATION_ALGORITHMS_TIMETABLE_HPP_
#define TRANSIT_NAVIGATION_ALGORITHMS_TIMETABLE_HPP_

#include "navigation/routing_algorithm.hpp"
#include "search/stop_to_line.hpp"
#include "timetable/timetable.hpp"

#include "id/stop.hpp"
#include "gtfs/time.hpp"

#include <boost/optional.hpp>

namespace transit
{
namespace navigation
{
namespace algorithm
{

class TimeTable : public RoutingAlgorithm
{
  public:
    TimeTable(timetable::TimeTable const &time_table, search::StopToLine const &stop_to_line);

    // query a route between two stops
    boost::optional<Trip> operator()(gtfs::Time const departure,
                                     StopID const origin,
                                     StopID const destination) const override final;

  private:
    // the unmodified timetable data to route on
    timetable::TimeTable const &time_table;

    // the look-up for lines from a given stop
    search::StopToLine const &stop_to_line;
};

} // namespace algorithm
} // namespace navigation
} // namespace transit

#endif // TRANSIT_NAVIGATION_ALGORITHMS_TIMETABLE_HPP_
