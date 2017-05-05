#ifndef TRANSIT_TIMETABLE_DEPARTURE_TABLE_FACTORY_HPP
#define TRANSIT_TIMETABLE_DEPARTURE_TABLE_FACTORY_HPP

#include "gtfs/frequency.hpp"
#include "gtfs/stop.hpp"

#include "timetable/departure_table.hpp"

#include <vector>

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

    // in case frequencies are not specified, we detect departures from the stop_time tables
    static DepartureTable produce(std::vector<gtfs::StopTime>::iterator begin,
                                  std::vector<gtfs::StopTime>::iterator end);
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_DEPARTURE_TABLE_FACTORY_HPP
