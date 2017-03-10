#include "timetable/timetable.hpp"

#include <algorithm>
#include <boost/assert.hpp>

namespace transit
{
namespace timetable
{

TimeTable::Trip::Trip(StopTable const &stops, DepartureTable const &departures)
    : stops(stops), departures(departures)
{
}

TimeTable::Trip TimeTable::get_trip(gtfs::TripID trip_id) const
{
    BOOST_ASSERT(static_cast<std::uint64_t>(trip_id) < stop_tables.size());

    return Trip{stop_tables[static_cast<std::uint64_t>(trip_id)],
                departure_tables[static_cast<std::uint64_t>(trip_id)]};
}

} // namespace timetable
} // namespace transit
