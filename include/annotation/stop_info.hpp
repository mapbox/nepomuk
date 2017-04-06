#ifndef TRANSIT_ANNOTATION_STOP_INFO_HPP_
#define TRANSIT_ANNOTATION_STOP_INFO_HPP_

#include "geometric/coordinate.hpp"
#include "gtfs/stop.hpp"
#include "id/dictionary.hpp"
#include "id/line.hpp"
#include "id/shape.hpp"
#include "id/stop.hpp"
#include "timetable/segment_table.hpp"
#include "timetable/line_table.hpp"

#include <cstddef>
#include <vector>
#include <boost/optional.hpp>

namespace transit
{
namespace annotation
{

struct ShapeOffsetLine
{
    ShapeID shape_id;
    std::size_t offset;
    LineID line_id;
};

struct StopInfo
{
    DictionaryID name_id;
    geometric::WGS84Coordinate location;
    std::vector<ShapeOffsetLine> shape_offset_line;
};

class StopInfoTable
{
  public:
    StopInfoTable(std::vector<gtfs::Stop> const &stops,
                  std::vector<boost::optional<ShapeID>> const &shape_from_line,
                  timetable::SegmentTable const &segments,
                  std::vector<timetable::LineTable> const &line_tables);

    StopInfo const &get_info(StopID const stop_id) const;

  private:
    std::vector<StopInfo> stop_info;
};

} // namespace annotation
} // namespace transit

#endif // TRANSIT_ANNOTATION_STOP_INFO_HPP_
