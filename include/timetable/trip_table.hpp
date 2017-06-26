#ifndef NEPOMUK_TIMETABLE_TRIP_TABLE_HPP_
#define NEPOMUK_TIMETABLE_TRIP_TABLE_HPP_

#include "timetable/duration_table.hpp"
#include "timetable/stop_table.hpp"

#include "date/time.hpp"
#include "id/trip.hpp"

#include <utility>
#include <vector>

namespace nepomuk
{

namespace annotation
{
class GeometryFactory;
} // namespace annotation

namespace timetable
{

struct Departures
{
    date::Time time;
    std::size_t stop_offset;
    std::size_t duration_offset;
};

// a view onto trips
class TripViewIterator
{
  public:
    TripViewIterator(GlobalStopTable::const_iterator stop_itr,
                     GlobalDurationTable::const_iterator durations_itr,
                     date::Time time);
    // Precrement the iterator
    TripViewIterator operator++();
    // Postcrement
    TripViewIterator operator++(int);

    StopID stop() const;
    date::Time arrival() const;
    date::Time departure() const;

    // checks the validity of the iterator
    operator bool() const;
    bool valid() const;
    bool has_next() const;

    // is this the first/last stop of the line
    bool is_first() const;
    bool is_last() const;

  private:
    // stops in order of their appearence on the trip
    GlobalStopTable::const_iterator stop_itr;
    // duration offsets in relation to the very first stop on the line
    GlobalDurationTable::const_iterator durations_itr;

    // the time the trip departs
    date::Time base;
};

class TripTable
{
  public:
    bool reachable(TripID const trip, std::size_t offset, date::Time departure) const;
    // only get trips after a certain departure
    TripViewIterator operator()(TripID const trip, std::size_t offset, date::Time departure) const;
    // all trips are fine
    TripViewIterator operator()(TripID const trip, std::size_t offset) const;

    std::pair<std::size_t, std::size_t> identifier(TripID const trip) const
    {
        return std::make_pair(departures[trip.base()].stop_offset,
                              departures[trip.base()].duration_offset);
    }

    // find the offset of a stop along at trip
    std::size_t offset(TripID const trip, StopID const stop) const;

  private:
    std::vector<Departures> departures;
    GlobalStopTable all_stops;
    GlobalDurationTable all_durations;

    friend class TripTableFactory;
    friend class TransferTableFactory;
    friend class StopToTrip;
    friend class nepomuk::annotation::GeometryFactory;
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_TRIP_TABLE_HPP_
