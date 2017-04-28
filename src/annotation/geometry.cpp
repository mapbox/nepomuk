#include "annotation/geometry.hpp"
#include "id/line.hpp"
#include "tool/status/progress.hpp"

#include <boost/range/iterator_range.hpp>

namespace transit
{
namespace annotation
{

Geometry::Geometry(timetable::SegmentTable const &segment_table) : segment_table(segment_table) {}

geometric::WGS84Coordinate Geometry::get(StopID const stop_id) const
{
    return stop_locations[stop_id.base()];
}

timetable::SegmentTable::const_iterator_range
Geometry::get(LineID const line, StopID const from, StopID const to) const
{
    // if the line isn't a line, return
    if (line == WALKING_TRANSFER || segment_table.category_size() <= line.base())
        return boost::make_iterator_range(segment_table.cend(), segment_table.cend());

    auto const from_info = shape_info.crange(from.base());
    auto const to_info = shape_info.crange(to.base());

    auto const has_desired_line = [line](auto const &sol) { return sol.line_id == line; };

    auto const from_sol_itr = std::find_if(from_info.begin(), from_info.end(), has_desired_line);
    auto const to_sol_itr = std::find_if(to_info.begin(), to_info.end(), has_desired_line);

    // make sure we find the line on both entries
    if ((from_sol_itr == from_info.end()) || (to_sol_itr == to_info.end()))
        return boost::make_iterator_range(segment_table.cend(), segment_table.cend());
    else
    {
        // TODO remove after resolving https://github.com/mapbox/directions-transit/issues/88
        if (from_sol_itr->offset > to_sol_itr->offset)
            return boost::make_iterator_range(segment_table.cend(), segment_table.cend());
        BOOST_ASSERT(from_sol_itr->shape_id == to_sol_itr->shape_id);
        BOOST_ASSERT(from_sol_itr->offset <= to_sol_itr->offset);
        // need to add +1 to the offset, since the offset is a start value, not end paradigm
        return segment_table.crange(
            from_sol_itr->shape_id.base(), from_sol_itr->offset, to_sol_itr->offset + 1);
    }
}

} // namespace annotation
} // namespace transit
