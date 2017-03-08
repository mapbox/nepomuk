#ifndef TRANSIT_TIMETABLE_HPP_
#define TRANSIT_TIMETABLE_HPP_

#include "timetable/departure_table.hpp"
#include "timetable/stop_table.hpp"

#include "gtfs/trip.hpp"

#include <boost/optional.hpp>

namespace transit
{
namespace timetable
{

class TimeTableFactory;

// The timetable class groups all the different tables together that make up the transit network. It
// essentially provides the graph and the travel times for the navigation algorithm
class TimeTable
{
  public:
    std::vector<StopTable> stop_tables;
    std::vector<DepartureTable> departure_tables;

    // a trip is specified by the list of stops along its path and the departures from the first
    // station
    struct Trip
    {
        const StopTable &stops;
        const DepartureTable &departures;

        Trip(StopTable const &stops, DepartureTable const &departures);
    };

    boost::optional<Trip> get_trip(gtfs::TripID trip_id) const;

  private:
    friend class TimeTableFactory;
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_HPP_
