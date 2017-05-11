#ifndef NEPOMUK_GEOMETRIC_POLYLINE_HPP_
#define NEPOMUK_GEOMETRIC_POLYLINE_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace nepomuk
{
namespace geometric
{

class WGS84Coordinate;

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
} // namespace nepomuk

#endif // NEPOMUK_GEOMETRIC_POLYLINE_HPP_
