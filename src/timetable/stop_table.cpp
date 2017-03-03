#include "timetable/stop_table.hpp"

namespace transit
{
namespace timetable
{

StopTable::const_iterator_range StopTable::list(gtfs::StopID /*start_id*/) const
{
    return boost::make_iterator_range(stop_arrivals.begin(), stop_arrivals.end());
}

} // namespace timetable
} // namespace transit
