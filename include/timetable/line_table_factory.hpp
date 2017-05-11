#ifndef NEPOMUK_TIMETABLE_LINE_TABLE_FACTORY_HPP_
#define NEPOMUK_TIMETABLE_LINE_TABLE_FACTORY_HPP_

// The line table factory groups all routes in the graph into lines. These lines only require a
// single stop table, which allows space efficient storage/better look-up of departures.

#include "gtfs/stop.hpp"
#include "id/shape.hpp"

#include <boost/optional.hpp>
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

class LineTableFactory
{
  public:
    LineTableFactory(std::vector<gtfs::Transfer> &transfers,
                     std::vector<gtfs::Trip> const &trips,
                     std::vector<boost::optional<ShapeID>> &shape_by_line);

    // create a line table from a list of stop_times
    std::vector<LineTable> produce(std::vector<gtfs::StopTime>::iterator const begin,
                                   std::vector<gtfs::StopTime>::iterator const end);

  private:
    std::vector<gtfs::Transfer> &transfers;
    // to look up shape ids
    std::vector<gtfs::Trip> const &trips;
    // to store mapping of shape ids
    std::vector<boost::optional<ShapeID>> &shape_by_line;
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_LINE_TABLE_FACTORY_HPP_
