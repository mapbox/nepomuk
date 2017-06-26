#ifndef NEPOMUK_TIMETABLE_TRANSFER_TABLE_FACTORY_HPP_
#define NEPOMUK_TIMETABLE_TRANSFER_TABLE_FACTORY_HPP_

#include "gtfs/transfer.hpp"
#include "search/coordinate_to_stop.hpp"
#include "timetable/transfer_table.hpp"

#include <cstddef>
#include <vector>

namespace nepomuk
{

namespace gtfs
{
struct Stop;
}

namespace timetable
{

class TripTable;
class StationTable;

class TransferTableFactory
{
  public:
    static TransferTable produce(std::vector<gtfs::Transfer>::iterator begin,
                                 std::vector<gtfs::Transfer>::iterator end);

    static TransferTable produce(std::vector<gtfs::Transfer>::iterator begin,
                                 std::vector<gtfs::Transfer>::iterator end,
                                 std::size_t const num_stops,
                                 TripTable const &trip_table,
                                 StationTable const &station_table,
                                 std::vector<gtfs::Stop> const &stops,
                                 search::CoordinateToStop const &stop_lookup);
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_TRANSFER_TABLE_FACTORY_HPP_
