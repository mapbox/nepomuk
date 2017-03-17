#ifndef TRANSIT_TIMETABLE_HPP_
#define TRANSIT_TIMETABLE_HPP_

#include "gtfs/trip.hpp"
#include "gtfs/stop.hpp"
#include "timetable/line_table.hpp"
#include "timetable/transfer_table.hpp"
#include "timetable/station_table.hpp"

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
    // a trip is specified by the list of stops along its path and the departures from the first
    // station
    auto list_trips(LineID const line, gtfs::Time const departure) const
    {
        BOOST_ASSERT(static_cast<std::uint64_t>(line) < line_tables.size());
        return line_tables[static_cast<std::uint64_t>(line)].get(departure);
    }

    auto list_transfers(gtfs::StopID const stop_id) const { return transfer_table.get(stop_id); }

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
