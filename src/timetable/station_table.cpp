#include "timetable/station_table.hpp"

#include <utility>

namespace transit
{
namespace timetable
{

StationTable::StationTable(Lookup stop_station_lookup)
    : stop_station_lookup(std::move(stop_station_lookup))
{
}

StationTable::StationTable() : stop_station_lookup(0, 0) {}

std::size_t StationTable::num_stops() const { return stop_station_lookup.domain_size(); }
std::size_t StationTable::num_stations() const { return stop_station_lookup.codomain_size(); }

} // namespace timetable
} // namespace transit
