#include "geometric/coordinate.hpp"

#include <algorithm>
#include <cmath>

namespace transit
{
namespace geometric
{

WGS84Coordinate::WGS84Coordinate(FixedLongitude longitude_, FixedLatitude latitude_)
    : longitude(longitude_), latitude(latitude_)
{
}

WGS84Coordinate::WGS84Coordinate()
    : longitude(makeLatLonFromDouble<FixedLongitude>(0.0)),
      latitude(makeLatLonFromDouble<FixedLatitude>(0.0))
{
}

bool WGS84Coordinate::valid() const
{
    auto const min_lon = makeLatLonFromDouble<FixedLongitude>(-180.0);
    auto const max_lon = makeLatLonFromDouble<FixedLongitude>(180.0);
    auto const min_lat = makeLatLonFromDouble<FixedLatitude>(-constants::EPSG3857_MAX_LATITUDE);
    auto const max_lat = makeLatLonFromDouble<FixedLatitude>(constants::EPSG3857_MAX_LATITUDE);

    return (min_lon <= longitude) && (longitude <= max_lon) && (min_lat <= latitude) &&
           (latitude <= max_lat);
}

void WGS84Coordinate::clamp()
{
    auto const min_lon = makeLatLonFromDouble<FixedLongitude>(-180.0);
    auto const max_lon = makeLatLonFromDouble<FixedLongitude>(180.0);
    auto const min_lat = makeLatLonFromDouble<FixedLatitude>(-constants::EPSG3857_MAX_LATITUDE);
    auto const max_lat = makeLatLonFromDouble<FixedLatitude>(constants::EPSG3857_MAX_LATITUDE);

    latitude = std::max(min_lat, std::min(max_lat, latitude));
    longitude = std::max(min_lon, std::min(max_lon, longitude));
}

std::ostream &operator<<(std::ostream &os, WGS84Coordinate const &location)
{
    os << static_cast<std::int32_t>(location.longitude) / COORDINATE_PRECISION << " "
       << static_cast<std::int32_t>(location.latitude) / COORDINATE_PRECISION;
    return os;
}

bool operator==(WGS84Coordinate const &lhs, WGS84Coordinate const &rhs)
{
    return lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude;
}

double distance(WGS84Coordinate const lhs, WGS84Coordinate const rhs)
{
    auto const as_double_in_rad = [](auto const value) {
        return doubleFromLatLon(value) * constants::degree_to_rad;
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
    return constants::EARTH_RADIUS * 2. * std::atan2(std::sqrt(aharv), std::sqrt(1.0 - aharv));
}

MercatorCoordinate::MercatorCoordinate()
    : longitude(makeLatLonFromDouble<FixedLongitude>(0.0)),
      latitude(makeLatLonFromDouble<FixedLatitude>(0.0))
{
}

MercatorCoordinate::MercatorCoordinate(WGS84Coordinate coordinate)
{
    coordinate.clamp();

    auto const longitude_radians =
        doubleFromLatLon(coordinate.longitude) * constants::degree_to_rad;

    longitude = makeLatLonFromDouble<FixedLongitude>(128.0 / M_PI * (longitude_radians + M_PI));

    auto const latitude_radians =
        (doubleFromLatLon(coordinate.latitude)) * constants::degree_to_rad;

    auto const sin_lat = std::sin(latitude_radians);
    latitude = makeLatLonFromDouble<FixedLatitude>(
        -((constants::rad_to_degree * std::log((1 + sin_lat) / (1 - sin_lat))) * 64. / 180. - 128));

    clamp();
}

void MercatorCoordinate::clamp()
{
    auto const min_lon = makeLatLonFromDouble<FixedLongitude>(0.0);
    auto const max_lon = makeLatLonFromDouble<FixedLongitude>(256.0);
    auto const min_lat = makeLatLonFromDouble<FixedLatitude>(0.0);
    auto const max_lat = makeLatLonFromDouble<FixedLatitude>(256.0);

    latitude = std::max(min_lat, std::min(max_lat, latitude));
    longitude = std::max(min_lon, std::min(max_lon, longitude));
}

std::ostream &operator<<(std::ostream &os, MercatorCoordinate const &location)
{
    os << static_cast<std::int32_t>(location.longitude) / COORDINATE_PRECISION << " "
       << static_cast<std::int32_t>(location.latitude) / COORDINATE_PRECISION;
    return os;
}

bool operator==(MercatorCoordinate const &lhs, MercatorCoordinate const &rhs)
{
    return lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude;
}

} // namespace geometric
} // namespace transit
