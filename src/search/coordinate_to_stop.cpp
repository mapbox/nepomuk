#include "search/coordinate_to_stop.hpp"

#include <algorithm>
#include <iostream>

namespace transit
{
namespace search
{

namespace
{

CoordinateToStop::PointT as_point(geometric::WGS84Coordinate location)
{
    return {geometric::doubleFromLatLon(location.longitude),
            geometric::doubleFromLatLon(location.latitude)};
}

boost::geometry::model::box<CoordinateToStop::PointT>
as_bbox(geometric::WGS84BoundingBox const &bounding_box)
{
    return {as_point(bounding_box.lower_left), as_point(bounding_box.upper_right)};
}

} // namespace

CoordinateToStop::CoordinateToStop(
    std::vector<std::pair<geometric::WGS84Coordinate, StopID>> const &coordinates)
{
    auto const add_to_rtree = [this](auto const &coordinate_and_stop) {
        tree.insert(
            std::make_pair(as_point(coordinate_and_stop.first), coordinate_and_stop.second));
    };
    std::for_each(coordinates.begin(), coordinates.end(), add_to_rtree);
}

StopID CoordinateToStop::nearest(geometric::WGS84Coordinate const &location) const
{
    std::vector<RTreeEntry> result;
    tree.query(boost::geometry::index::nearest(as_point(location), 1), std::back_inserter(result));
    return result.front().second;
}

std::vector<StopID> CoordinateToStop::all(geometric::WGS84BoundingBox const &bounding_box) const
{
    std::vector<RTreeEntry> result;
    tree.query(boost::geometry::index::intersects(as_bbox(bounding_box)),
               std::back_inserter(result));
    std::vector<StopID> only_stops(result.size());
    std::transform(result.begin(), result.end(), only_stops.begin(), [](auto const &pair) {
        return pair.second;
    });
    return only_stops;
}

} // namespace search
} // namespace transit
