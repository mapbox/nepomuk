#ifndef NEPOMUK_GTFS_ACCESSIBILITY_HPP_
#define NEPOMUK_GTFS_ACCESSIBILITY_HPP_

#include <string>

namespace nepomuk
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
} // namespace nepomuk

#endif // NEPOMUK_GTFS_ACCESSIBILITY_HPP_
