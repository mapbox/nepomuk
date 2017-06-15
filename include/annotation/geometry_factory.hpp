#ifndef NEPOMUK_ANNOTATION_GEOMETRY_FACTORY_HPP_
#define NEPOMUK_ANNOTATION_GEOMETRY_FACTORY_HPP_

#include <vector>

#include "annotation/geometry.hpp"
#include "timetable/segment_table.hpp"

namespace nepomuk
{
namespace gtfs
{
struct Stop;
struct Trip;
} // namespace gtfs

namespace timetable
{
class LineTable;
} // namespace timetable

struct ShapeID;

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
                            std::vector<gtfs::Trip> const &trips,
                            std::vector<std::size_t> const &trip_offsets_by_line,
                            search::StopToLine const &stop_to_line,
                            std::vector<timetable::LineTable> const &line_tables,
                            timetable::SegmentTable const &segment_table);
};

} // namespace annotation
} // namespace nepomuk

#endif // NEPOMUK_ANNOTATION_GEOMETRY_FACTORY_HPP_
