#include "timetable/stop_table.hpp"

namespace transit
{
namespace timetable
{

bool StopTable::Arrival::operator==(StopTable::Arrival const &other) const
{
    return stop_id == other.stop_id && delta_t == other.delta_t;
}

StopTable::const_iterator_range StopTable::list(gtfs::StopID /*start_id*/) const
{
    return boost::make_iterator_range(stop_arrivals.begin(), stop_arrivals.end());
}

bool StopTable::operator==(StopTable const &other) const
{
    return std::equal(stop_arrivals.begin(),
                      stop_arrivals.end(),
                      other.stop_arrivals.begin(),
                      other.stop_arrivals.end());
}

} // namespace timetable
} // namespace transit
