#include "timetable/stop_to_trip.hpp"
#include "timetable/trip_table.hpp"

#include <algorithm>
#include <boost/assert.hpp>

namespace nepomuk
{
namespace timetable
{

StopToTrip::StopToTrip(std::size_t const num_stops, TripTable const &trip_table)
{
    trips_by_stop.resize(num_stops);

    std::size_t index = 0;
    // for each stop, remember the trips leaving it and the offset along the trip data
    for (auto const &departure : trip_table.departures)
    {
        auto itr = trip_table.all_stops.begin() + departure.stop_offset;
        std::size_t offset = 0;
        while (*itr != StopID::INVALID())
        {
            trips_by_stop[itr->base()].push_back({TripID{index}, offset});
            ++offset;
            ++itr;
        }
        ++index;
    }
}

std::vector<TripOffset> const &StopToTrip::operator()(StopID const stop) const
{
    BOOST_ASSERT(stop.base() < trips_by_stop.size());
    return trips_by_stop[stop.base()];
}

} // namespace timetable
} // namespace nepomuk
