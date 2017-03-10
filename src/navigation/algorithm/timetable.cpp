#include "navigation/algorithm/timetable.hpp"
#include "navigation/leg.hpp"

#include "gtfs/stop.hpp"
#include "gtfs/time.hpp"
#include "gtfs/trip.hpp"

namespace transit
{
namespace navigation
{
namespace algorithm
{

TimeTable::TimeTable(timetable::TimeTable const &time_table, search::StopToLine const &stop_to_line)
    : time_table(time_table), stop_to_line(stop_to_line)
{
}

Trip TimeTable::operator()(gtfs::Time const departure,
                           gtfs::StopID const origin,
                           gtfs::StopID const destination) const
{
    Trip result;

    auto trip_range = stop_to_line(origin);
    for (auto trip_id : trip_range)
    {
        auto trip = time_table.get_trip(trip_id);
        for (auto const trip_departure : trip.departures.list(departure))
        {
            Leg leg;

            set_departure(leg, trip_departure.getNextDeparture(departure));
            for (auto const stop : trip.stops.list(origin))
                add_stop(leg, stop);

            add_leg(result, std::move(leg));
        }
    }
    return result;
}

} // namespace algorithm
} // namespace navigation
} // namespace transit
