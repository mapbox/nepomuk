#include "search/coordinate_to_stop.hpp"

#include <algorithm>
#include <boost/geometry/geometries/box.hpp>

namespace transit
{
namespace search
{

namespace
{

CoordinateToStop::PointT as_point(geometric::WGS84Coordinate const location)
{
    return {geometric::doubleFromLatLon(location.longitude),
            geometric::doubleFromLatLon(location.latitude)};
}

geometric::WGS84Coordinate as_coordinate(CoordinateToStop::PointT const point)
{
    return {geometric::makeLatLonFromDouble<geometric::FixedLongitude>(point.get<0>()),
            geometric::makeLatLonFromDouble<geometric::FixedLatitude>(point.get<1>())};
}

boost::geometry::model::box<CoordinateToStop::PointT>
as_bbox(geometric::WGS84BoundingBox const &bounding_box)
{
    return {as_point(bounding_box.lower_left), as_point(bounding_box.upper_right)};
}

boost::geometry::model::box<CoordinateToStop::PointT>
box_around_circle(geometric::WGS84Coordinate const location, double const radius)
{
    // TODO: ensure this does work at the borders of the map
    auto scale_coordinate_lon =
        geometric::WGS84Coordinate(geometric::makeLatLonFromDouble<geometric::FixedLongitude>(
                                       geometric::doubleFromLatLon(location.longitude) + 0.1),
                                   location.latitude);
    auto scale_coordinate_lat =
        geometric::WGS84Coordinate(location.longitude,
                                   geometric::makeLatLonFromDouble<geometric::FixedLatitude>(
                                       geometric::doubleFromLatLon(location.latitude) + 0.1));

    auto const scale_x = 0.1 / geometric::distance(scale_coordinate_lon, location);
    auto const scale_y = 0.1 / geometric::distance(scale_coordinate_lat, location);

    geometric::WGS84Coordinate lower_left = {
        geometric::makeLatLonFromDouble<geometric::FixedLongitude>(
            geometric::doubleFromLatLon(location.longitude) - 0.5 * radius * scale_x),
        geometric::makeLatLonFromDouble<geometric::FixedLatitude>(
            geometric::doubleFromLatLon(location.latitude) - 0.5 * radius * scale_y)};
    geometric::WGS84Coordinate upper_right = {
        geometric::makeLatLonFromDouble<geometric::FixedLongitude>(
            geometric::doubleFromLatLon(location.longitude) + 0.5 * radius * scale_x),
        geometric::makeLatLonFromDouble<geometric::FixedLatitude>(
            geometric::doubleFromLatLon(location.latitude) + 0.5 * radius * scale_y)};

    return {as_point(lower_left), as_point(upper_right)};
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

CoordinateToStop::value_type
CoordinateToStop::nearest(geometric::WGS84Coordinate const &location) const
{
    std::vector<RTreeEntry> result;
    tree.query(boost::geometry::index::nearest(as_point(location), 1), std::back_inserter(result));
    return {result.front().second, as_coordinate(result.front().first)};
}

std::vector<CoordinateToStop::value_type>
CoordinateToStop::nearest(geometric::WGS84Coordinate const &location, std::uint32_t count) const
{
    std::vector<RTreeEntry> result;
    tree.query(boost::geometry::index::nearest(as_point(location), count),
               std::back_inserter(result));
    std::vector<value_type> as_value_type(result.size());
    std::transform(
        result.begin(), result.end(), as_value_type.begin(), [](auto const &pair) -> value_type {
            return {pair.second, as_coordinate(pair.first)};
        });
    return as_value_type;
}

std::vector<CoordinateToStop::value_type>
CoordinateToStop::all(geometric::WGS84Coordinate const &location, double const radius) const
{
    std::vector<RTreeEntry> result;
    tree.query(boost::geometry::index::intersects(box_around_circle(location, 1.1 * radius)),
               std::back_inserter(result));

    // sort results by distance to the location
    std::sort(result.begin(), result.end(), [location](auto const &lhs, auto const &rhs) {
        return geometric::distance(location, as_coordinate(lhs.first)) <
               geometric::distance(location, as_coordinate(rhs.first));
    });

    result.erase(std::find_if(result.begin(),
                              result.end(),
                              [location, radius](auto const &entry) {
                                  return geometric::distance(location, as_coordinate(entry.first)) >
                                         radius;
                              }),
                 result.end());

    std::vector<value_type> as_value_type(result.size());
    std::transform(
        result.begin(), result.end(), as_value_type.begin(), [](auto const &pair) -> value_type {
            return {pair.second, as_coordinate(pair.first)};
        });
    return as_value_type;
}

std::vector<CoordinateToStop::value_type>
CoordinateToStop::all(geometric::WGS84BoundingBox const &bounding_box) const
{
    std::vector<RTreeEntry> result;
    tree.query(boost::geometry::index::intersects(as_bbox(bounding_box)),
               std::back_inserter(result));
    std::vector<value_type> as_value_type(result.size());
    std::transform(
        result.begin(), result.end(), as_value_type.begin(), [](auto const &pair) -> value_type {
            return {pair.second, as_coordinate(pair.first)};
        });
    return as_value_type;
}

} // namespace search
} // namespace transit
