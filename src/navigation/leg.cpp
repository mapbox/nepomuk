#include "navigation/leg.hpp"

namespace transit
{
namespace navigation
{

boost::iterator_range<Leg::iterator> Leg::list() const
{
    return boost::make_iterator_range(stops.begin(), stops.end());
}

gtfs::Time Leg::departure() const { return _departure; }

LineID Leg::line() const { return _line; }

} // namespace navigation
} // namespace transit
