#include "navigation/leg.hpp"

namespace transit
{
namespace navigation
{

boost::iterator_range<Leg::iterator> Leg::list() const
{
    return boost::make_iterator_range(stops.begin(),stops.end());
}

gtfs::Time Leg::departure() const
{
    return _departure;
}

} // namespace navigation
} // namespace transit
