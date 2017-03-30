#include "timetable/line_table.hpp"

#include <set>

namespace transit
{
namespace timetable
{

LineTable::Trip::Trip(StopTable const &stop_table,
                      DurationTable const &duration_table,
                      gtfs::Time const departure)
    : stop_table(stop_table), duration_table(duration_table), departure(departure)
{
}

boost::optional<LineTable::Trip> LineTable::get(gtfs::Time const desired_departure) const
{
    auto const departure = departures.list(desired_departure);
    if (departure.begin() == departure.end())
        return boost::none;

    Trip result = {stop_table,
                   duration_tables[departure.begin()->duration_table_index],
                   departure.begin()->get_next_departure(desired_departure)};

    return result;
}

StopTable const& LineTable::stops() const
{
    return stop_table;
}

} // namespace timetable
} // namespace transit
