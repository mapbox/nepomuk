#include "annotation/geometry_factory.hpp"

#include "gtfs/stop.hpp"
#include "id/line.hpp"
#include "id/shape.hpp"
#include "id/stop.hpp"
#include "timetable/line_table.hpp"
#include "tool/status/progress.hpp"

#include <algorithm>
#include <boost/assert.hpp>
#include <cstddef>
#include <cstdint>
#include <iterator>

namespace nepomuk
{
namespace annotation
{

Geometry GeometryFactory::produce(std::vector<gtfs::Stop> const &stops,
                                  std::vector<boost::optional<ShapeID>> const &shape_from_line,
                                  search::StopToLine const &stop_to_line,
                                  std::vector<timetable::LineTable> const &line_tables,
                                  timetable::SegmentTable const &segment_table)
{
    tool::status::FunctionTimingGuard timing_guard("Geometry Annotation Creation");

    struct StopLineOffset
    {
        StopID stop;
        LineID line;
        std::size_t offset;
    };
    std::vector<StopLineOffset> shape_info;

    { // to not clutter the scope
        tool::status::ProgressBarGuard guard(line_tables.size());
        auto const add_offsets_for_stops =
            [&, index = std::uint64_t{0} ](auto const &line_table) mutable
        {
            // only actually add shapes, if there is a shape
            if (shape_from_line[index])
            {
                auto const shape_index = shape_from_line[index]->base();
                BOOST_ASSERT(shape_index < segment_table.category_size());
                auto range = segment_table.crange(shape_index);
                auto itr = range.begin();
                for (auto stop : line_table.stops().list())
                {
                    itr = std::min_element(itr, range.end(), [&](auto const lhs, auto const rhs) {
                        return geometric::distance(stops[stop.base()].location, lhs) <
                               geometric::distance(stops[stop.base()].location, rhs);

                    });
                    BOOST_ASSERT(stop.base() < stops.size());
                    shape_info.push_back(
                        {stop,
                         LineID{index},
                         static_cast<std::size_t>(std::distance(range.begin(), itr))});
                }
            }
            guard.print(++index);
        };
        // locate nodes in segments
        std::for_each(line_tables.begin(), line_tables.end(), add_offsets_for_stops);
    }

    auto const by_stop_id = [](auto const &lhs, auto const &rhs) { return lhs.stop < rhs.stop; };
    std::sort(shape_info.begin(), shape_info.end(), by_stop_id);

    Geometry geometry_annotation(segment_table);
    geometry_annotation.stop_locations.reserve(stops.size());

    std::transform(stops.begin(),
                   stops.end(),
                   std::back_inserter(geometry_annotation.stop_locations),
                   [](auto const &stop) { return stop.location; });

    {
        std::size_t index = 0;
        tool::status::ProgressBarGuard guard(shape_info.size());
        auto const add_stop_geometry = [&](auto const &stop_line_offset) {
            geometry_annotation.shape_info.push_back(
                stop_line_offset.stop.base(),
                {stop_line_offset.line,
                 *shape_from_line[stop_line_offset.line.base()],
                 stop_line_offset.offset});
            guard.print(++index);
        };

        // locate nodes in segments
        std::for_each(shape_info.begin(), shape_info.end(), add_stop_geometry);
    }

    return geometry_annotation;
}

} // namespace annotation
} // namespace nepomuk
