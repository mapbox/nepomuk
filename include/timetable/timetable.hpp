#ifndef NEPOMUK_TIMETABLE_HPP_
#define NEPOMUK_TIMETABLE_HPP_

#include "id/stop.hpp"
#include "timetable/station_table.hpp"
#include "timetable/stop_to_trip.hpp"
#include "timetable/transfer_table.hpp"
#include "timetable/trip_table.hpp"

#include <vector>

namespace nepomuk
{

namespace search
{
class StopToLineFactory;
} // namespace search

namespace timetable
{

// The timetable class groups all the different tables together that make up the transit network. It
// essentially provides the graph and the travel times for the navigation algorithm
class TimeTable
{
  public:
    // get a line by its id
    // get all proposed transfers at a stop
    TransferTable::iterator_range transfers(StopID const stop) const;
    auto const &trip_table() const { return _trip_table; }

    // map stations to/from stops
    auto station(StopID const stop_id) const { return station_table.station(stop_id); }
    auto stops(StopID const station_id) const { return station_table.stops(station_id); }

    auto num_stops() const { return station_table.num_stops(); }
    auto num_stations() const { return station_table.num_stations(); }
  private:
    TripTable _trip_table;

    TransferTable transfer_table;
    StationTable station_table;

    friend class TimeTableFactory;
    friend class nepomuk::search::StopToLineFactory;
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_HPP_
