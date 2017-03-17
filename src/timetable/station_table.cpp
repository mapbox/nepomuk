#include "timetable/station_table.hpp"

namespace transit
{
namespace timetable
{

StationTable::StationTable(Lookup stop_station_lookup)
    : stop_station_lookup(std::move(stop_station_lookup))
{
}

StationTable::StationTable()
    : stop_station_lookup(0,0)
{
}

} // namespace timetable
} // namespace transit
