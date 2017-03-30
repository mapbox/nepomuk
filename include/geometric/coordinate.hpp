#ifndef TRANSIT_GEOMETRIC_COORDINATE_HPP_
#define TRANSIT_GEOMETRIC_COORDINATE_HPP_

#include <tool/types_and_traits/strong_typedef.hpp>

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

namespace constants
{
const constexpr long double degree_to_rad = 0.017453292519943295769236907684886;
const constexpr long double rad_to_degree = (1.0 / 0.017453292519943295769236907684886);
const constexpr long double EARTH_RADIUS = 6372797.560856;
const constexpr long double MAXEXTENT = EARTH_RADIUS * M_PI;
const constexpr double EPSG3857_MAX_LATITUDE = 85.051128779806592377;
} // namespace

STRONG_ID_TYPE(std::int32_t, FixedLatitude)
STRONG_ID_TYPE(std::int32_t, FixedLongitude)

template <typename lat_or_long> lat_or_long makeLatLonFromDouble(double const value)
{
    return lat_or_long{
        static_cast<typename lat_or_long::value_type>(std::round(COORDINATE_PRECISION * value))};
}

template <typename lat_or_long> double doubleFromLatLon(lat_or_long const value)
{
    return static_cast<std::int32_t>(value) / COORDINATE_PRECISION;
}

class WGS84Coordinate
{
  public:
    WGS84Coordinate(FixedLongitude, FixedLatitude);
    WGS84Coordinate();

    friend std::ostream &operator<<(std::ostream &os, WGS84Coordinate const &location);
    friend bool operator==(WGS84Coordinate const &lhs, WGS84Coordinate const &rhs);
    friend bool operator!=(WGS84Coordinate const &lhs, WGS84Coordinate const &rhs);

    FixedLongitude longitude;
    FixedLatitude latitude;

    bool valid() const;
    void clamp();
};

class MercatorCoordinate
{
  public:
    MercatorCoordinate();
    MercatorCoordinate(WGS84Coordinate coordinate);

    FixedLongitude longitude;
    FixedLatitude latitude;

    void clamp();

    friend std::ostream &operator<<(std::ostream &os, WGS84Coordinate const &location);
    friend bool operator==(WGS84Coordinate const &lhs, WGS84Coordinate const &rhs);
    friend bool operator!=(WGS84Coordinate const &lhs, WGS84Coordinate const &rhs);
};

double distance(WGS84Coordinate const lhs, WGS84Coordinate const rhs);

std::ostream &operator<<(std::ostream &os, WGS84Coordinate const &location);
bool operator==(WGS84Coordinate const &lhs, WGS84Coordinate const &rhs);
bool operator!=(WGS84Coordinate const &lhs, WGS84Coordinate const &rhs);

std::ostream &operator<<(std::ostream &os, MercatorCoordinate const &location);
bool operator==(MercatorCoordinate const &lhs, MercatorCoordinate const &rhs);
bool operator!=(MercatorCoordinate const &lhs, MercatorCoordinate const &rhs);
} // namespace geometric
} // namespace transit

#endif // TRANSIT_GEOMETRIC_COORDINATE_HPP_
