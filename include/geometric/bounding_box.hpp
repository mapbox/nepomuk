#ifndef TRANSIT_GEOMETRIC_BOUNDING_BOX_HPP_
#define TRANSIT_GEOMETRIC_BOUNDING_BOX_HPP_

#include "geometric/coordinate.hpp"
#include <cstdint>

namespace transit
{
namespace geometric
{

class WGS84BoundingBox
{
  public:
    WGS84BoundingBox(WGS84Coordinate lower_left, WGS84Coordinate upper_right);
    WGS84BoundingBox(std::uint32_t const horizontal,
                     std::uint32_t const vertical,
                     std::uint32_t const zoom_level,
                     double const tile_size = 256.0,
                     std::int32_t const mercator_buffer = 0);

    bool contains(WGS84Coordinate const &coordinate) const;

    // simple difference in lat/lon
    double width() const;
    double height() const;

    WGS84Coordinate lower_left;
    WGS84Coordinate upper_right;
};

class MercatorBoundingBox
{
  public:
    MercatorBoundingBox(MercatorCoordinate lower_left, MercatorCoordinate upper_right);
    MercatorBoundingBox(std::uint32_t const horizontal,
                        std::uint32_t const vertical,
                        std::uint32_t const zoom_level,
                        double const tile_size = 256.0,
                        std::int32_t const mercator_buffer = 0);

    bool contains(MercatorCoordinate const &coordinate) const;

    double width() const;
    double height() const;

    MercatorCoordinate lower_left;
    MercatorCoordinate upper_right;
};

} // namespace geometric
} // namespace transit

#endif // TRANSIT_GEOMETRIC_BOUNDING_BOX_HPP_
