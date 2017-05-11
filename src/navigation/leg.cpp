#include "navigation/leg.hpp"

#include <boost/assert.hpp>

namespace nepomuk
{
namespace navigation
{

date::UTCTimestamp Leg::departure() const
{
    BOOST_ASSERT(!_segments.empty());
    return _segments.front().departure();
}

date::UTCTimestamp Leg::arrival() const
{
    BOOST_ASSERT(!_segments.empty());
    return _segments.back().arrival();
}

std::uint32_t Leg::duration() const { return arrival() - departure(); }

} // namespace navigation
} // namespace nepomuk
