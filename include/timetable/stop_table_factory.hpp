#ifndef NEPOMUK_TIMETABLE_STOP_TABLE_FACTORY_HPP_
#define NEPOMUK_TIMETABLE_STOP_TABLE_FACTORY_HPP_

#include "timetable/stop_table.hpp"

#include "gtfs/stop.hpp"

#include <vector>

namespace nepomuk
{
namespace timetable
{

class StopTableFactory
{
  public:
    static StopTable produce(std::vector<gtfs::StopTime> &stop_times);

    static StopTable produce(std::vector<gtfs::StopTime>::iterator begin,
                             const std::vector<gtfs::StopTime>::iterator end);
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_STOP_TABLE_FACTORY_HPP_
