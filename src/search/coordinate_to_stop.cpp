#include "search/coordinate_to_stop.hpp"

#include <algorithm>
#include <iostream>

namespace transit
{
namespace search
{

CoordinateToStop::CoordinateToStop(
    std::vector<std::pair<geometric::Coordinate, gtfs::StopID>> coordinates)
    : coordinates(std::move(coordinates))
{
}

gtfs::StopID CoordinateToStop::nearest(geometric::Coordinate const &location) const
{
    // find the closes element to location
    auto itr = std::min_element(coordinates.begin(),
                            coordinates.end(),
                            [location](auto const &lhs, auto const &rhs) {
                                return geometric::distance(lhs.first, location) <
                                       geometric::distance(rhs.first, location);
                            });

    std::cout << "Found closest stop " << itr->second << " at " << itr->first << std::endl;
    return itr->second;
}

} // namespace search
} // namespace transit
