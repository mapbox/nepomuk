#include "navigation/route.hpp"

#include <boost/assert.hpp>

namespace transit
{
namespace navigation
{

date::UTCTimestamp Route::departure() const
{
    BOOST_ASSERT(!_legs.empty());
    return _legs.front().departure();
}

date::UTCTimestamp Route::arrival() const
{
    BOOST_ASSERT(!_legs.empty());
    return _legs.back().arrival();
}

std::uint32_t Route::duration() const { return arrival() - departure(); }

} // namespace navigation
} // namespace transit
