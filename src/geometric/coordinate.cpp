#include "geometric/coordinate.hpp"

namespace transit
{
namespace geometric
{

Coordinate::Coordinate(FixedLongitude longitude_, FixedLatitude latitude_)
    : longitude(longitude_), latitude(latitude_)
{
}

} // namespace geometric
} // namespace transit
