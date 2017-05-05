#include "geometric/bounding_box.hpp"

#include <algorithm>
#include <cmath>       // for atan, exp
#include <type_traits> // for move, swap

#include <boost/math/constants/constants.hpp>

namespace transit
{
namespace geometric
{

WGS84BoundingBox::WGS84BoundingBox(WGS84Coordinate lower_left, WGS84Coordinate upper_right)
    : lower_left(std::move(lower_left)), upper_right(std::move(upper_right))
{
}

WGS84BoundingBox::WGS84BoundingBox(std::uint32_t const horizontal,
                                   std::uint32_t const vertical,
                                   std::uint32_t const zoom_level,
                                   double const tile_size,
                                   std::int32_t const mercator_buffer)
{
    double min_lon = horizontal * tile_size - mercator_buffer;
    double max_lon = (horizontal + 1.0) * tile_size + mercator_buffer;

    double min_lat = (vertical + 1.0) * tile_size + mercator_buffer;
    double max_lat = vertical * tile_size - mercator_buffer;

    // 2^z * TILE_SIZE
    const double shift = (1u << static_cast<unsigned>(zoom_level)) * tile_size;

    auto const shift_lon = [shift](double const lon) {
        return (lon - 0.5 * shift) / shift * 360.0;
    };

    auto const shift_lat = [shift](double const lat) {
        auto const tmp =
            boost::math::constants::pi<double>() * constants::rad_to_degree * (1 - 2 * lat / shift);

        auto const clamped = std::max<long double>(-180., std::min<long double>(180., tmp));
        auto const normalized =
            constants::rad_to_degree * 2. * std::atan(std::exp(clamped * constants::degree_to_rad));

        return normalized - 90.;
    };

    min_lon = shift_lon(min_lon);
    max_lon = shift_lon(max_lon);
    min_lat = shift_lat(min_lat);
    max_lat = shift_lat(max_lat);

    lower_left = WGS84Coordinate(makeLatLonFromDouble<FixedLongitude>(min_lon),
                                 makeLatLonFromDouble<FixedLatitude>(min_lat));
    upper_right = WGS84Coordinate(makeLatLonFromDouble<FixedLongitude>(max_lon),
                                  makeLatLonFromDouble<FixedLatitude>(max_lat));
}

bool WGS84BoundingBox::contains(WGS84Coordinate const &coordinate) const
{
    return lower_left.longitude <= coordinate.longitude &&
           coordinate.longitude <= upper_right.longitude &&
           lower_left.latitude <= coordinate.latitude &&
           coordinate.latitude <= upper_right.latitude;
}

double WGS84BoundingBox::width() const
{
    return doubleFromLatLon(upper_right.longitude) - doubleFromLatLon(lower_left.longitude);
}

double WGS84BoundingBox::height() const
{
    return doubleFromLatLon(upper_right.latitude) - doubleFromLatLon(lower_left.latitude);
}

MercatorBoundingBox::MercatorBoundingBox(MercatorCoordinate lower_left_,
                                         MercatorCoordinate upper_right_)
    : lower_left(lower_left_), upper_right(upper_right_)
{
    // mercator transformations switch invert latitude (counting from upper right). If the
    // coordinates don't match up (e.g. due to conversion issues) we need to swap them to the
    // correct order
    if (lower_left.latitude > upper_right.latitude)
        std::swap(lower_left.latitude, upper_right.latitude);
}

MercatorBoundingBox::MercatorBoundingBox(std::uint32_t const horizontal,
                                         std::uint32_t const vertical,
                                         std::uint32_t const zoom_level,
                                         double const tile_size,
                                         std::int32_t const mercator_buffer)
{
    WGS84BoundingBox bbox(horizontal, vertical, zoom_level, tile_size, mercator_buffer);
    lower_left = MercatorCoordinate(bbox.lower_left);
    upper_right = MercatorCoordinate(bbox.upper_right);
    std::swap(lower_left.latitude, upper_right.latitude);
}

bool MercatorBoundingBox::contains(MercatorCoordinate const &coordinate) const
{
    return lower_left.longitude <= coordinate.longitude &&
           coordinate.longitude <= upper_right.longitude &&
           lower_left.latitude <= coordinate.latitude &&
           coordinate.latitude <= upper_right.latitude;
}

double MercatorBoundingBox::width() const
{
    return doubleFromLatLon(upper_right.longitude) - doubleFromLatLon(lower_left.longitude);
}
double MercatorBoundingBox::height() const
{
    return doubleFromLatLon(upper_right.latitude) - doubleFromLatLon(lower_left.latitude);
}

} // namespace geometric
} // namespace transit
