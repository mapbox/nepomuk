#include "geometric/bounding_box.hpp"

namespace transit
{
namespace geometric
{

BoundingBox::BoundingBox(Coordinate lower_left, Coordinate upper_right)
    : lower_left(std::move(lower_left)), upper_right(std::move(upper_right))
{
}

bool BoundingBox::contains(Coordinate const &coordinate) const
{
    return lower_left.longitude <= coordinate.longitude &&
           coordinate.longitude <= upper_right.longitude &&
           lower_left.latitude <= coordinate.latitude &&
           coordinate.latitude <= upper_right.latitude;
}

} // namespace geometric
} // namespace transit
