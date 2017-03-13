#ifndef TRANSIT_TIMETABLE_HPP_
#define TRANSIT_TIMETABLE_HPP_

#include "timetable/line_table.hpp"

#include "gtfs/trip.hpp"

namespace transit
{
namespace search
{
class StopToLineFactory;
} // namespace search

namespace timetable
{

class TimeTableFactory;

// The timetable class groups all the different tables together that make up the transit network. It
// essentially provides the graph and the travel times for the navigation algorithm
class TimeTable
{
  public:
    // a trip is specified by the list of stops along its path and the departures from the first
    // station
    LineTable::Trip get(LineID const line, gtfs::Time const departure) const;

  private:
    std::vector<LineTable> line_tables;

    friend class TimeTableFactory;
    friend class transit::search::StopToLineFactory;
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_HPP_
