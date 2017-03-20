#ifndef TRANSIT_GEOMETRIC_BOUNDING_BOX_HPP_
#define TRANSIT_GEOMETRIC_BOUNDING_BOX_HPP_

#include "geometric/coordinate.hpp"

namespace transit
{
namespace geometric
{

class BoundingBox
{
  public:
    BoundingBox(Coordinate lower_left, Coordinate upper_right);
    bool contains(Coordinate const &coordinate) const;

  private:
    Coordinate lower_left;
    Coordinate upper_right;
};

} // namespace geometric
} // namespace transit

#endif // TRANSIT_GEOMETRIC_BOUNDING_BOX_HPP_
