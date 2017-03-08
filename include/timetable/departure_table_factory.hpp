#ifndef TRANSIT_TIMETABLE_DEPARTURE_TABLE_FACTORY_HPP
#define TRANSIT_TIMETABLE_DEPARTURE_TABLE_FACTORY_HPP

#include "gtfs/frequency.hpp"
#include "timetable/departure_table.hpp"

namespace transit
{
namespace timetable
{

class DepartureTableFactory
{
  public:
    // create the departure table of stop_time based on frequencies
    static DepartureTable produce(std::vector<gtfs::Frequency>::iterator begin,
                                  std::vector<gtfs::Frequency>::iterator end);
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_DEPARTURE_TABLE_FACTORY_HPP
