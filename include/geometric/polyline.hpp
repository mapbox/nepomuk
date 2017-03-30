#ifndef TRANSIT_GEOMETRIC_POLYLINE_HPP_
#define TRANSIT_GEOMETRIC_POLYLINE_HPP_

#include "geometric/coordinate.hpp"
#include <cstdint>
#include <vector>

namespace transit
{
namespace geometric
{

// Polyline encoding (See:
// https://developers.google.com/maps/documentation/utilities/polylinealgorithm)
class Polyline
{
  public:
    static std::string encode(std::uint32_t precision,
                              std::vector<WGS84Coordinate> const &coordinates);
    static std::vector<WGS84Coordinate> decode(std::uint32_t preciison,
                                               std::string const &polyline);
};

} // namespace geometric
} // namespace transit

#endif // TRANSIT_GEOMETRIC_POLYLINE_HPP_
