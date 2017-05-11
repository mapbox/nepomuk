#ifndef NEPOMUK_TIMETABLE_STATION_TABLE_FACTORY_HPP_
#define NEPOMUK_TIMETABLE_STATION_TABLE_FACTORY_HPP_

#include "gtfs/stop.hpp"
#include "timetable/station_table.hpp"

#include <vector>

namespace nepomuk
{
namespace timetable
{
class StationTableFactory
{
  public:
    static StationTable produce(std::vector<gtfs::Stop>::iterator const begin,
                                std::vector<gtfs::Stop>::iterator const end);
};
} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_STATION_TABLE_FACTORY_HPP_
