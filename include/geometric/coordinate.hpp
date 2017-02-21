#ifndef TRANSIT_GEOMETRIC_COORDINATE_HPP_
#define TRANSIT_GEOMETRIC_COORDINATE_HPP_

#include <boost/serialization/strong_typedef.hpp>
#include <cmath>
#include <cstdint>

namespace transit
{
namespace geometric
{

namespace
{
constexpr auto const COORDINATE_PRECISION = 1.0e06;
}

BOOST_STRONG_TYPEDEF(std::uint32_t, FixedLatitude)
BOOST_STRONG_TYPEDEF(std::uint32_t, FixedLongitude)

template <typename lat_or_long> lat_or_long makeLatLonFromDouble(double const value)
{
    return lat_or_long(std::round(COORDINATE_PRECISION * value));
}

class Coordinate
{
  public:
    Coordinate(FixedLongitude, FixedLatitude);

    FixedLongitude longitude;
    FixedLongitude latitude;
};

} // namespace geometric
} // namespace transit

#endif // TRANSIT_GEOMETRIC_COORDINATE_HPP_
