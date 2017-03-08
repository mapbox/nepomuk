#include "timetable/timetable.hpp"

#include <algorithm>

namespace transit
{
namespace timetable
{

TimeTable::Trip::Trip(StopTable const &stops, DepartureTable const &departures)
    : stops(stops), departures(departures)
{
}

boost::optional<TimeTable::Trip> TimeTable::get_trip(gtfs::TripID trip_id) const
{
    auto const by_id = [trip_id](auto const &table) { return trip_id == table.trip_id(); };

    auto const stop = std::find_if(stop_tables.begin(), stop_tables.end(), by_id);

    if (stop == stop_tables.end())
        return boost::none;

    auto const departure = std::find_if(departure_tables.begin(), departure_tables.end(), by_id);

    if (departure == departure_tables.end())
        return boost::none;
    else
        return Trip{*stop, *departure};
}

} // namespace timetable
} // namespace transit
