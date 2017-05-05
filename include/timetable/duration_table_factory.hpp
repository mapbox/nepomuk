#ifndef TRANSIT_TIMETABLE_DURATION_TABLE_FACTORY_HPP_
#define TRANSIT_TIMETABLE_DURATION_TABLE_FACTORY_HPP_

#include "gtfs/stop.hpp"
#include "timetable/duration_table.hpp"

#include <vector>

namespace transit
{
namespace timetable
{

class DurationTableFactory
{
  public:
    static DurationTable produce(std::vector<gtfs::StopTime>::iterator begin,
                                 const std::vector<gtfs::StopTime>::iterator end);
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_DURATION_TABLE_FACTORY_HPP_
