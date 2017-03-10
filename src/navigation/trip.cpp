#include "navigation/trip.hpp"

namespace transit
{
namespace navigation
{

boost::iterator_range<Trip::iterator> Trip::list() const
{
    return boost::make_iterator_range(legs.begin(), legs.end());
}

} // namespace navigation
} // namespace transit
