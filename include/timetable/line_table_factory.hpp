#ifndef NEPOMUK_TIMETABLE_LINE_TABLE_FACTORY_HPP_
#define NEPOMUK_TIMETABLE_LINE_TABLE_FACTORY_HPP_

// The line table factory groups all routes in the graph into lines. These lines only require a
// single stop table, which allows space efficient storage/better look-up of departures.

#include "gtfs/stop.hpp"
#include "id/shape.hpp"

#include <vector>

namespace nepomuk
{

namespace gtfs
{
struct Transfer;
struct Trip;
}

namespace timetable
{

class LineTable;

// Stop trimes provide trips that all reach the same stops in order. These are transferred into
// lines that we can follow. Each line follows the same sequence of stops. We identify lines to
// offer the same information.
class LineTableFactory
{
  public:
    LineTableFactory(std::vector<gtfs::Transfer> &transfers,
                     std::vector<std::size_t> &trip_offsets_by_line);

    // create a line table from a list of stop_times
    std::vector<LineTable> produce(std::vector<gtfs::StopTime>::iterator const begin,
                                   std::vector<gtfs::StopTime>::iterator const end);

  private:
    std::vector<gtfs::Transfer> &transfers;

    // offset into the trip array so that each trip represents a full set of lines
    std::vector<std::size_t> &trip_offsets_by_line;
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_LINE_TABLE_FACTORY_HPP_
