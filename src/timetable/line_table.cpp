#include "timetable/line_table.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>

#include <boost/assert.hpp>

namespace nepomuk
{
namespace timetable
{

boost::optional<LineTable::Trip> LineTable::get(StopID const stop,
                                                date::Time const desired_departure) const
{
    auto const stop_range = stop_table.list(stop);
    // how far are we into the stop table
    auto const offset = std::distance(stop_table.list().begin(), stop_range.begin());

    // compute the initial arrival time, looking back:
    auto const offset_departure =
        desired_departure - std::min(desired_departure.seconds_since_midnight, time_deltas[offset]);

    auto const local_departures = departures.list(offset_departure);

    // find the next departure that actually is departing at a time larger, or equal to the desired
    // departure
    auto const get_line_departure = [&](Departure const &departure) {
        BOOST_ASSERT(departure.duration_table_index < duration_tables.size());
        auto time_delta = duration_tables[departure.duration_table_index].prefix_time(offset);
        auto line_depart_time =
            desired_departure - std::min(desired_departure.seconds_since_midnight, time_delta);
        if (departure.end < line_depart_time)
            return date::Time::infinity();
        return departure.get_next_departure(line_depart_time);
    };

    auto itr = std::min_element(local_departures.begin(),
                                local_departures.end(),
                                [&](Departure const &lhs, Departure const &rhs) {
                                    return get_line_departure(lhs) < get_line_departure(rhs);
                                });

    if (itr == local_departures.end() || itr->end < get_line_departure(*itr))
        return boost::none;

    BOOST_ASSERT(itr->duration_table_index < duration_tables.size());
    Trip result = {stop_range,
                   duration_tables[itr->duration_table_index].list(offset),
                   get_line_departure(*itr) +
                       duration_tables[itr->duration_table_index].prefix_time(offset)};

    return result;
}

StopTable const &LineTable::stops() const { return stop_table; }

} // namespace timetable
} // namespace nepomuk
