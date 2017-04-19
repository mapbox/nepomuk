#ifndef TRANSIT_SEARCH_COORDINATE_TO_STOP_HPP_
#define TRANSIT_SEARCH_COORDINATE_TO_STOP_HPP_

#include "geometric/bounding_box.hpp"
#include "geometric/coordinate.hpp"
#include "id/stop.hpp"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>

#include <boost/geometry/index/rtree.hpp>

#include <cstdint>
#include <utility>
#include <vector>

namespace transit
{
namespace search
{

class CoordinateToStop
{
  public:
    CoordinateToStop(std::vector<std::pair<geometric::WGS84Coordinate, StopID>> const&);

    using value_type = std::pair<StopID,geometric::WGS84Coordinate>;

    // find the nearest stop to a location
    value_type nearest(geometric::WGS84Coordinate const &location) const;
    std::vector<value_type> nearest(geometric::WGS84Coordinate const &location, std::uint32_t count) const;
    std::vector<value_type> all(geometric::WGS84BoundingBox const &bounding_box) const;
    std::vector<value_type> all(geometric::WGS84Coordinate const &location, double const radius) const;

    using PointT = boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>;
  private:
    using RTreeEntry = std::pair<PointT, StopID>;
    using RTree = boost::geometry::index::rtree<RTreeEntry, boost::geometry::index::quadratic<64>>;

    RTree tree;
};

} // namespace search
} // namespace transit

#endif // TRANSIT_SEARCH_COORDINATE_TO_STOP_HPP_
