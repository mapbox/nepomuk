#ifndef TRANSIT_TIMETABLE_TRANSFER_TABLE_FACTORY_HPP_
#define TRANSIT_TIMETABLE_TRANSFER_TABLE_FACTORY_HPP_

#include "gtfs/transfer.hpp"
#include "search/coordinate_to_stop.hpp"
#include "timetable/transfer_table.hpp"

#include <cstddef>
#include <vector>

namespace transit
{

namespace gtfs
{
struct Stop;
}

namespace timetable
{

class LineTable;

class TransferTableFactory
{
  public:
    static TransferTable produce(std::vector<gtfs::Transfer>::iterator begin,
                                 std::vector<gtfs::Transfer>::iterator end);

    static TransferTable produce(std::vector<gtfs::Transfer>::iterator begin,
                                 std::vector<gtfs::Transfer>::iterator end,
                                 std::size_t const num_stops,
                                 std::vector<LineTable> const &line_tables,
                                 std::vector<gtfs::Stop> const &stops,
                                 search::CoordinateToStop const &stop_lookup);
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_TRANSFER_TABLE_FACTORY_HPP_
