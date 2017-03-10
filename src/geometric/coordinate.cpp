#include "geometric/coordinate.hpp"

namespace transit
{
namespace geometric
{

Coordinate::Coordinate(FixedLongitude longitude_, FixedLatitude latitude_)
    : longitude(longitude_), latitude(latitude_)
{
}

std::ostream &operator<<(std::ostream &os, Coordinate const &location)
{
    os << static_cast<std::int32_t>(location.longitude) / COORDINATE_PRECISION << " "
       << static_cast<std::int32_t>(location.latitude) / COORDINATE_PRECISION;
    return os;
}

bool operator==(Coordinate const &lhs, Coordinate const &rhs)
{
    return lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude;
}

} // namespace geometric
} // namespace transit
