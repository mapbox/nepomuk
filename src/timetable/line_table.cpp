#include "timetable/line_table.hpp"

#include <set>

namespace transit
{
namespace timetable
{

LineTable::Trip::Trip(StopTable const *const stop_table,
                      DurationTable const *const duration_table,
                      gtfs::Time const departure)
    : stop_table(stop_table), duration_table(duration_table), departure(departure)
{
}

std::vector<LineTable::Trip> LineTable::get(gtfs::Time const desired_departure) const
{
    auto const departure_range = departures.list(desired_departure);

    std::vector<Trip> result;
    std::set<std::size_t> stop_indices;

    result.reserve(std::distance(departure_range.begin(), departure_range.end()));
    std::for_each(
        departure_range.begin(),
        departure_range.end(),
        [ this, desired_departure, &stop_indices, destination = std::back_inserter(result) ](
            auto const &departure) mutable {
            if (!stop_indices.count(departure.stop_table_index))
            {
                // todo add index to departure list
                *destination = Trip(&stop_tables[departure.stop_table_index],
                                    &duration_tables[departure.duration_table_index],
                                    departure.get_next_departure(desired_departure));
                destination++;
                stop_indices.insert(departure.stop_table_index);
            }
        });
    return result;
}

} // namespace timetable
} // namespace transit
