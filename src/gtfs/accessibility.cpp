#include "gtfs/accessibility.hpp"

#include <boost/assert.hpp>

namespace transit
{
namespace gtfs
{
namespace accessibility
{

Wheelchair makeWheelchair(std::string const &value)
{
    if (value.empty() || value == "0")
        return Wheelchair::INHERIT_OR_NONE;
    else if (value == "1")
        return Wheelchair::SOME_OR_ALL;
    else
    {
        BOOST_ASSERT(value == "2");
        return Wheelchair::NONE;
    }
}

} // namespace accessibility
} // namespace gtfs
} // namespace transit
