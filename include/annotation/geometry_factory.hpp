#ifndef TRANSIT_ANNOTATION_GEOMETRY_FACTORY_HPP_
#define TRANSIT_ANNOTATION_GEOMETRY_FACTORY_HPP_

#include <boost/optional.hpp>
#include <vector>

namespace transit
{
namespace gtfs
{
struct Stop;
}
}
namespace transit
{
namespace timetable
{
class LineTable;
}
}
namespace transit
{
struct ShapeID;
}

#include "annotation/geometry.hpp"
#include "timetable/segment_table.hpp"

namespace transit
{
namespace search
{
class StopToLine;
} // namespace search

namespace annotation
{

class GeometryFactory
{
  public:
    static Geometry produce(std::vector<gtfs::Stop> const &stops,
                            std::vector<boost::optional<ShapeID>> const &shape_from_line,
                            search::StopToLine const &stop_to_line,
                            std::vector<timetable::LineTable> const &line_tables,
                            timetable::SegmentTable const &segment_table);
};

} // namespace annotation
} // namespace transit

#endif // TRANSIT_ANNOTATION_GEOMETRY_FACTORY_HPP_
