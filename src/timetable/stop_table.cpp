#include "timetable/stop_table.hpp"

#include <algorithm>

namespace transit
{
namespace timetable
{

StopTable::const_iterator_range StopTable::list(StopID const start_id) const
{
    return boost::make_iterator_range(
        // locate the start_id within the stops
        std::find_if(stops.begin(),
                     stops.end(),
                     [start_id](auto const &entry) { return start_id == entry; }),
        stops.end());
}

StopTable::const_iterator_range StopTable::list() const
{
    return boost::make_iterator_range(stops.begin(), stops.end());
}

bool StopTable::operator==(StopTable const &other) const
{
    return std::equal(stops.begin(), stops.end(), other.stops.begin(), other.stops.end());
}

} // namespace timetable
} // namespace transit
