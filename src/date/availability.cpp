#include "date/availability.hpp"

#include <boost/assert.hpp>
#include <type_traits>

namespace nepomuk
{
namespace date
{

Availability::Availability(Weekday::Enum const flags)
: flags(flags)
{}

// Representation of the weekly flags in GTFS data for monday through sunday
bool Availability::operator()(Weekday::Enum const weekday) const
{
    BOOST_ASSERT(weekday < 128);
    return (weekday & flags) == weekday;
}

} // namespace date
} // namespace nepomuk
