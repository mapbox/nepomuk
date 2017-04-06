#include "annotation/stop_info.hpp"

#include <algorithm>
#include <boost/assert.hpp>
#include <cstdint>
#include <iterator>

namespace transit
{
namespace annotation
{

StopInfoTable::StopInfoTable(std::vector<gtfs::Stop> const &stops,
                             std::vector<boost::optional<ShapeID>> const &shape_from_line,
                             timetable::SegmentTable const &segments,
                             std::vector<timetable::LineTable> const &line_tables)
{
    stop_info.resize(stops.size());

    // set the names
    std::transform(
        stops.begin(), stops.end(), stop_info.begin(), [this](auto const &stop) -> StopInfo {
            BOOST_ASSERT(stop_info.size() == static_cast<std::uint64_t>(stop.id));
            return {stop.name.base(), stop.location, {}};
        });

    auto const add_offsets_to_nodes = [&, index = std::uint64_t{0} ](auto const &line_table) mutable
    {
        // only actually add shapes, if there is a shape
        if (shape_from_line[index])
        {
            auto const shape_index = shape_from_line[index]->base();
            auto range = segments.crange(shape_index);
            auto itr = range.begin();
            for (auto stop : line_table.stops().list())
            {
                itr = std::min_element(itr, range.end(), [&](auto const lhs, auto const rhs) {
                    return geometric::distance(stop_info[stop.base()].location, lhs) <
                           geometric::distance(stop_info[stop.base()].location, rhs);
                });
                stop_info[stop.base()].shape_offset_line.push_back(
                    {shape_index,
                     static_cast<std::size_t>(std::distance(range.begin(), itr)),
                     index});
            }
        }
        ++index;
    };

    // locate nodes in segments
    std::for_each(line_tables.begin(), line_tables.end(), add_offsets_to_nodes);
}

StopInfo const &StopInfoTable::get_info(StopID const stop_id) const
{
    return stop_info[stop_id.base()];
}

} // namespace annotation
} // namespace transit
