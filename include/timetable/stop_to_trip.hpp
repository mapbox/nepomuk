#ifndef NEPOMUK_TIMETABLE_STOP_TO_TRIP_HPP_
#define NEPOMUK_TIMETABLE_STOP_TO_TRIP_HPP_

#include <cstddef>
#include <vector>

#include "id/stop.hpp"
#include "id/trip.hpp"

namespace nepomuk
{
namespace timetable
{

class TripTable;

// an ID mapping to the trip table, as well as an offset (which stop along the trip)
struct TripOffset
{
    TripID trip_id;
    std::size_t offset;
};

class StopToTrip
{
  public:
    StopToTrip(std::size_t const num_stops, TripTable const &timetable);

    std::vector<TripOffset> const &operator()(StopID const stop) const;

  private:
    // for all stops it lists the number of available trips in order of their departures
    std::vector<std::vector<TripOffset>> trips_by_stop;
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_STOP_TO_TRIP_HPP_
