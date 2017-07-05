#ifndef NEPOMUK_ANNOTATION_LEG_GEOMETRY_HPP_
#define NEPOMUK_ANNOTATION_LEG_GEOMETRY_HPP_

#include <cstddef>
#include <vector>

#include "geometric/coordinate.hpp"
#include "id/shape.hpp"
#include "id/stop.hpp"
#include "id/trip.hpp"
#include "timetable/segment_table.hpp"
#include "tool/container/indexed_vector.hpp"

#include <boost/optional.hpp>

namespace nepomuk
{
namespace annotation
{

class Geometry
{
  public:
    geometric::WGS84Coordinate get(StopID const stop) const;

    timetable::SegmentTable::const_iterator_range
    get(TripID const line, std::size_t from_offset, std::size_t to_offset) const;

  private:
    Geometry(timetable::SegmentTable const &segment_table);
    friend class GeometryFactory;

    // accessing the route-id from a trip id
    std::vector<boost::optional<ShapeID>> shape_by_trip;

    // the geometry offsets, in sync with the all_stops of the trip table
    std::vector<std::size_t> geometry_offsets;

    // the segment table provides geometries for segments. They can be accessed via a shape_id
    // (shape_id_by_trip). The offsets along the shape are stored in the geometry_offsets array
    timetable::SegmentTable const &segment_table;

    // every stop offers a dedicated coordinate, even if segments are not present
    std::vector<geometric::WGS84Coordinate> stop_locations;
};

} // namespace annotation
} // namespace nepomuk

#endif // NEPOMUK_ANNOTATION_LEG_GEOMETRY_HPP_
