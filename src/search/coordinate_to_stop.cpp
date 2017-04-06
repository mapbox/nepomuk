#include "search/coordinate_to_stop.hpp"

#include <algorithm>
#include <iostream>

namespace transit
{
namespace search
{

CoordinateToStop::CoordinateToStop(
    std::vector<std::pair<geometric::WGS84Coordinate, StopID>> coordinates)
    : coordinates(std::move(coordinates))
{
}

StopID CoordinateToStop::nearest(geometric::WGS84Coordinate const &location) const
{
    // find the closes element to location
    auto itr = std::min_element(
        coordinates.begin(), coordinates.end(), [location](auto const &lhs, auto const &rhs) {
            return geometric::distance(lhs.first, location) <
                   geometric::distance(rhs.first, location);
        });

    return itr->second;
}

std::vector<StopID> CoordinateToStop::all(geometric::WGS84BoundingBox const &bounding_box) const
{
    std::vector<StopID> results;
    std::for_each(coordinates.begin(),
                  coordinates.end(),
                  [&results, bounding_box](auto const &coordinate_stop) {
                      if (bounding_box.contains(coordinate_stop.first))
                          results.push_back(coordinate_stop.second);
                  });
    return results;
}

} // namespace search
} // namespace transit
