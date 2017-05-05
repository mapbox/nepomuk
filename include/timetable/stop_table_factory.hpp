#include "timetable/stop_table.hpp"

#include "gtfs/stop.hpp"

#include <vector>

namespace transit
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
} // namespace transit
