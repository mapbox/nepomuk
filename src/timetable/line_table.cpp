#include "timetable/line_table.hpp"

namespace transit
{
namespace timetable
{

LineTable::Trip::Trip(StopTable const &stop_table, gtfs::Time const departure)
    : stop_table(stop_table), departure(departure)
{
}

LineTable::Trip LineTable::get(gtfs::Time const desired_departure) const
{
    auto const list_of_departures = departures.list(desired_departure).begin();
    return Trip(serviced_stops, departures.list(desired_departure).begin()->begin);
}

} // namespace timetable
} // namespace transit
