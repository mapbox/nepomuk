#include "timetable/departure_table.hpp"

#include <algorithm>

#include <boost/assert.hpp>

namespace nepomuk
{
namespace timetable
{

bool DepartureTable::Departure::operator<(DepartureTable::Departure const &other) const
{
    return end < other.end;
}

date::Time DepartureTable::Departure::get_next_departure(date::Time const starting_at) const
{
    // the next departue for a given starting time is defined by T = begin + x * headway with T >=
    // starting_at and x minimal among all x that fulfill T >= starting_at

    BOOST_ASSERT(starting_at <= end);

    // in case the trip is only serviced after the starting time, the very first service can be
    // reached. After this check starting_at is within the range [begin,end)
    if (starting_at <= begin)
        return begin;

    // compute the xth train that can be reached (ceil(delta_t/headway))
    const auto x = (starting_at - begin + headway - 1) / headway;

    return begin + x * headway;
}

DepartureTable::const_iterator_range DepartureTable::list(date::Time starting_at) const
{
    return {std::lower_bound(departures.begin(),
                             departures.end(),
                             starting_at,
                             [](auto const &departure, auto const &time) {
                                 return departure.end < time + (departure.headway);
                             }),
            departures.end()};
}

} // namespace timetable
} // namespace nepomuk
