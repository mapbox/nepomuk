#ifndef TRANSIT_NAVIGATION_ROUTING_ALGORITHM_HPP
#define TRANSIT_NAVIGATION_ROUTING_ALGORITHM_HPP

#include "date/time.hpp"
#include "id/line.hpp"
#include "id/stop.hpp"
#include "navigation/leg.hpp"
#include "navigation/trip.hpp"

#include "timetable/line_table.hpp"

#include <boost/optional.hpp>
#include <cstdint>
#include <vector>

namespace transit
{
namespace navigation
{

// interface for all routing algorithms to implement
class RoutingAlgorithm
{
  public:
    // Arrival/Departure leg that provides information about origin stops/destination stops
    struct ADLeg
    {
        StopID stop;
        std::uint32_t seconds;
        double meters;
    };

    RoutingAlgorithm(std::vector<timetable::LineTable> const &line_tables);

    virtual boost::optional<Trip>
    operator()(date::Time const departure, StopID const origin, StopID const destination) const = 0;

    virtual boost::optional<Trip> operator()(date::Time const departure,
                                     std::vector<ADLeg> const &origins,
                                     std::vector<ADLeg> const &destinations) const = 0;

  protected:
    std::vector<timetable::LineTable> const &line_tables;

    // General Routing Structures to be used by all reference implementations
    struct PathEntry
    {
        StopID stop;
        LineID line;
        date::Time arrival;
        date::Time departure;
    };

    // since many lines can validly depart a stop but only one stop can be the best parent of a
    // station, we store the departure of the parent with the destination. This allows to find the
    // actual departure of the trip that reaches a station/stop. To report our results, we now need
    // to update the departures by shifting them one to the front.
    void update_departures_and_arrivals(std::vector<PathEntry> &path) const;
    Trip make_trip(std::vector<PathEntry> path) const;

    // routing algorithms (to avoid problems with intermediate stops changing after traversing a
    // line) only return a packed path that contains no intermediate stops. Using this method, we
    // can add back all the intermediate stops to return a full set of information.
    std::vector<PathEntry> unpack_path(std::vector<PathEntry> packed_path) const;

    // functionality to store the data in legs/trips. Necessary so we only have a single friend for
    // the private members. The different algorithms can only use this uniform interface to store
    // information.
    void add_leg(Trip &trip, Leg leg) const;
    void add_stop(Leg &leg, Leg::stop_type stop) const;
    void set_departure(Leg &leg, date::Time time) const;
    void set_line(Leg &leg, LineID line) const;
};

} // namespace navigation
} // namespace transit

#endif // TRANSIT_NAVIGATION_ROUTING_ALGORITHM_HPP
