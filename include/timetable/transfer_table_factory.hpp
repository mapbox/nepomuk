#ifndef TRANSIT_TIMETABLE_TRANSFER_TABLE_FACTORY_HPP_
#define TRANSIT_TIMETABLE_TRANSFER_TABLE_FACTORY_HPP_

#include "gtfs/transfer.hpp"
#include "search/stop_to_line.hpp"
#include "timetable/line_table.hpp"
#include "timetable/transfer_table.hpp"

namespace transit
{
namespace timetable
{

class TransferTableFactory
{
  public:
    static TransferTable produce(std::vector<gtfs::Transfer>::iterator begin,
                                 std::vector<gtfs::Transfer>::iterator end);

    static TransferTable produce(std::vector<gtfs::Transfer>::iterator begin,
                                 std::vector<gtfs::Transfer>::iterator end,
                                 std::size_t const num_stops,
                                 std::vector<LineTable> const &line_tables);
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_TRANSFER_TABLE_FACTORY_HPP_
