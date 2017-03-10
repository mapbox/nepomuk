#include "search/stop_to_line.hpp"

namespace transit
{
namespace search
{

StopToLine::iterator_range StopToLine::operator()(gtfs::StopID stop) const
{
    return boost::make_iterator_range(trip_table[static_cast<std::uint64_t>(stop)].begin(),
                                      trip_table[static_cast<std::uint64_t>(stop)].end());
}

} // namespace search
} // namespace transit
