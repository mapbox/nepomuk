#ifndef NEPOMUK_ANNOTATION_LEG_GEOMETRY_HPP_
#define NEPOMUK_ANNOTATION_LEG_GEOMETRY_HPP_

#include <cstddef>
#include <vector>

#include "geometric/coordinate.hpp"
#include "id/line.hpp"
#include "id/shape.hpp"
#include "id/stop.hpp"
#include "timetable/segment_table.hpp"
#include "tool/container/indexed_vector.hpp"

namespace nepomuk
{
namespace annotation
{

class Geometry
{
  public:
    // All information required to look up the gometry between two stops on a line. Shape and Offset
    // are indices into the segment table, which contains coordinates categorized by line ids
    struct LineShapeOffset
    {
        LineID line_id;
        ShapeID shape_id;
        std::size_t offset;
    };

    geometric::WGS84Coordinate get(StopID const stop) const;

    timetable::SegmentTable::const_iterator_range
    get(LineID const line, StopID const from, StopID const to) const;

  private:
    Geometry(timetable::SegmentTable const &segment_table);
    friend class GeometryFactory;

    timetable::SegmentTable const &segment_table;

    std::vector<geometric::WGS84Coordinate> stop_locations;
    tool::container::IndexedVector<LineShapeOffset> shape_info;
};

} // namespace annotation
} // namespace nepomuk

#endif // NEPOMUK_ANNOTATION_LEG_GEOMETRY_HPP_
