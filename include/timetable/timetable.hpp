#ifndef TRANSIT_TIMETABLE_HPP_
#define TRANSIT_TIMETABLE_HPP_

#include "gtfs/stop.hpp"
#include "gtfs/trip.hpp"
#include "timetable/line_table.hpp"
#include "timetable/station_table.hpp"
#include "timetable/transfer_table.hpp"

#include <boost/assert.hpp>
#include <vector>

namespace transit
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
    LineTable const& line(LineID const& line) const;

    // get all proposed transfers at a stop
    TransferTable::iterator_range transfers(gtfs::StopID const stop) const;

    // map stations to/from stops
    auto station(gtfs::StopID const stop_id) const { return station_table.station(stop_id); }
    auto stops(gtfs::StopID const station_id) const { return station_table.stops(station_id); }

  private:
    std::vector<LineTable> line_tables;
    TransferTable transfer_table;
    StationTable station_table;

    friend class TimeTableFactory;
    friend class transit::search::StopToLineFactory;
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_HPP_
