#include "timetable/trip_table.hpp"
#include <boost/assert.hpp>

#include <algorithm>
#include <limits>

namespace nepomuk
{
namespace timetable
{

TripViewIterator::TripViewIterator(GlobalStopTable::const_iterator stop_itr,
                                   GlobalDurationTable::const_iterator durations_itr,
                                   date::Time time)
    : stop_itr(stop_itr), durations_itr(durations_itr), base(time)
{
}

// precrement iterator
TripViewIterator TripViewIterator::operator++()
{
    durations_itr += 2;
    ++stop_itr;
    return *this;
}

// postcrement operator
TripViewIterator TripViewIterator::operator++(int)
{
    TripViewIterator copy = *this;
    durations_itr += 2;
    ++stop_itr;
    return copy;
}

StopID TripViewIterator::stop() const
{
    BOOST_ASSERT(static_cast<bool>(this));
    return *stop_itr;
}
date::Time TripViewIterator::arrival() const { return base + *durations_itr; }
date::Time TripViewIterator::departure() const { return base + *(durations_itr + 1); }

// checks the validity of the iterator
TripViewIterator::operator bool() const { return *stop_itr != StopID::INVALID(); }
bool TripViewIterator::valid() const { return static_cast<bool>(*this); }
bool TripViewIterator::has_next() const { return !is_last(); }

bool TripViewIterator::is_first() const
{
    return *durations_itr == std::numeric_limits<std::uint32_t>::max();
}
bool TripViewIterator::is_last() const
{
    return *(durations_itr + 1) == std::numeric_limits<std::uint32_t>::max();
}

bool TripTable::reachable(TripID const trip, std::size_t offset, date::Time const departure) const
{
    auto const &trip_info = departures[trip.base()];
    // we store 2 values per station (arrival + departure)
    auto const total_offset = trip_info.duration_offset + 2 * offset + 1;
    return departure <= trip_info.time + *(all_durations.begin() + total_offset);
}

std::size_t TripTable::offset(TripID const trip, StopID const stop) const
{
    // locate the stop along the line, report total offset
    auto const begin = all_stops.begin() + departures[trip.base()].stop_offset;
    auto const end = all_stops.end();
    return std::distance(begin, std::find(begin, end, stop));
}

std::size_t TripTable::offset(TripID const trip) const
{
    // locate the stop along the line, report total offset
    return departures[trip.base()].stop_offset;
}

TripViewIterator TripTable::
operator()(TripID const trip, std::size_t offset, date::Time departure) const
{
    (void)departure;
    BOOST_ASSERT(reachable(trip, offset, departure));
    return (*this)(trip, offset);
}

TripViewIterator TripTable::operator()(TripID const trip, std::size_t offset) const
{
    auto const &trip_info = departures[trip.base()];
    // list to the following stops (including self)
    // list to the following arrivals + departures (including self)
    // base departure time of the trip
    auto const stop_offset = trip_info.stop_offset + offset;
    auto const duration_offset = trip_info.duration_offset + 2 * offset;
    BOOST_ASSERT(stop_offset < all_stops.size());
    BOOST_ASSERT(duration_offset < all_durations.size());
    return {
        all_stops.begin() + stop_offset, all_durations.begin() + duration_offset, trip_info.time};
}

TripViewIterator TripTable::operator()(TripID const trip, StopID const stop) const
{
    return (*this)(trip, offset(trip, stop));
}

TripViewIterator TripTable::
operator()(TripID const trip, StopID const stop, date::Time const departure) const
{
    return (*this)(trip, offset(trip, stop), departure);
}

} // namespace timetable
} // namespace nepomuk
