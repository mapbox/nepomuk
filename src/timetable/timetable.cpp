#include "timetable/timetable.hpp"

#include <algorithm>
#include <boost/assert.hpp>

namespace transit
{
namespace timetable
{

std::vector<LineTable::Trip> TimeTable::get(LineID const line, gtfs::Time const departure) const
{
    BOOST_ASSERT(static_cast<std::uint64_t>(line) < line_tables.size());
    return line_tables[static_cast<std::uint64_t>(line)].get(departure);
}

} // namespace timetable
} // namespace transit
