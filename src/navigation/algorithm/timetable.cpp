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

TimeTable::TimeTable(timetable::TimeTable const &time_table) : time_table(time_table) {}

Trip TimeTable::operator()(gtfs::Time const departure,
                           gtfs::StopID const origin,
                           gtfs::StopID const destination) const
{
    Trip result;
    auto trip = time_table.get_trip(gtfs::TripID{0});
    if (trip)
    {
        for (auto const trip_departure : trip->departures.list(departure))
        {
            Leg leg;

            set_departure(leg, trip_departure.getNextDeparture(departure));
            for (auto const stop : trip->stops.list(origin))
                add_stop(leg, stop);

            add_leg(result, std::move(leg));
        }
    }
    return result;
}

} // namespace algorithm
} // namespace navigation
} // namespace transit
