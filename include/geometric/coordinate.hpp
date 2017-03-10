#ifndef TRANSIT_GEOMETRIC_COORDINATE_HPP_
#define TRANSIT_GEOMETRIC_COORDINATE_HPP_

#include <boost/serialization/strong_typedef.hpp>

#include <cmath>
#include <cstdint>
#include <iostream>

namespace transit
{
namespace geometric
{

namespace
{
constexpr auto const COORDINATE_PRECISION = 1.0e06;
}

BOOST_STRONG_TYPEDEF(std::int32_t, FixedLatitude)
BOOST_STRONG_TYPEDEF(std::int32_t, FixedLongitude)

template <typename lat_or_long> lat_or_long makeLatLonFromDouble(double const value)
{
    return lat_or_long(std::round(COORDINATE_PRECISION * value));
}

class Coordinate
{
  public:
    Coordinate(FixedLongitude, FixedLatitude);

    friend std::ostream &operator<<(std::ostream &os, Coordinate const &location);
    friend bool operator==(Coordinate const&lhs, Coordinate const&rhs);

    FixedLongitude longitude;
    FixedLongitude latitude;
};

std::ostream &operator<<(std::ostream &os, Coordinate const &location);
bool operator==(Coordinate const&lhs, Coordinate const&rhs);

} // namespace geometric
} // namespace transit

#endif // TRANSIT_GEOMETRIC_COORDINATE_HPP_
