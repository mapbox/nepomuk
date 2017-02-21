#ifndef TRANSIT_GTFS_ACCESSIBILITY_HPP_
#define TRANSIT_GTFS_ACCESSIBILITY_HPP_

#include <cstdint>
#include <string>

namespace transit
{
namespace gtfs
{
namespace accessibility
{

enum class Wheelchair
{
    INHERIT_OR_NONE,
    SOME_OR_ALL,
    NONE
};

Wheelchair makeWheelchair(std::string const &value);

} // namespace accessibility
} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_ACCESSIBILITY_HPP_
