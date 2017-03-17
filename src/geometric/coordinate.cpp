#include "geometric/coordinate.hpp"
#include <cmath>

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

double distance(Coordinate const lhs, Coordinate const rhs)
{
    const constexpr long double degree_to_rad = 0.017453292519943295769236907684886;
    const constexpr long double EARTH_RADIUS = 6372797.560856;
    auto const as_double_in_rad = [](auto const value) {
        return static_cast<std::uint32_t>(value) / COORDINATE_PRECISION * degree_to_rad;
    };

    const double lhs_latitude = as_double_in_rad(lhs.latitude);
    const double lhs_longitude = as_double_in_rad(lhs.longitude);
    const double rhs_latitude = as_double_in_rad(rhs.latitude);
    const double rhs_longitude = as_double_in_rad(rhs.longitude);

    const double delta_longitude = lhs_longitude - rhs_longitude;
    const double delta_latitude = lhs_latitude - rhs_latitude;

    const double aharv = std::pow(std::sin(delta_latitude / 2.0), 2.0) +
                         std::cos(lhs_latitude) * std::cos(rhs_latitude) *
                             std::pow(std::sin(delta_longitude / 2.), 2);
    return EARTH_RADIUS * 2. * std::atan2(std::sqrt(aharv), std::sqrt(1.0 - aharv));
}

} // namespace geometric
} // namespace transit
