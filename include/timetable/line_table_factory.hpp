#ifndef TRANSIT_TIMETABLE_LINE_TABLE_FACTORY_HPP_
#define TRANSIT_TIMETABLE_LINE_TABLE_FACTORY_HPP_

// The line table factory groups all routes in the graph into lines. These lines only require a
// single
// stop table, which allows space efficient storage/better look-up of departures.

#include "timetable/line_table.hpp"

#include "gtfs/stop.hpp"
#include <vector>

namespace transit
{
namespace timetable
{

class LineTableFactory
{
  public:
    // create a line table from a list of stop_times
    static std::vector<LineTable> produce(std::vector<gtfs::StopTime>::iterator const begin,
                                          std::vector<gtfs::StopTime>::iterator const end);
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_LINE_TABLE_FACTORY_HPP_
