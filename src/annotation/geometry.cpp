#include "annotation/geometry.hpp"
#include "id/line.hpp"

#include <algorithm>                           // for find_if
#include <boost/assert.hpp>                    // for BOOST_ASSERT
#include <boost/range/iterator_range_core.hpp> // for iterator_range

namespace nepomuk
{
namespace annotation
{

Geometry::Geometry(timetable::SegmentTable const &segment_table) : segment_table(segment_table) {}

geometric::WGS84Coordinate Geometry::get(StopID const stop_id) const
{
    BOOST_ASSERT(stop_id.base() < stop_locations.size());
    return stop_locations[stop_id.base()];
}

timetable::SegmentTable::const_iterator_range
Geometry::get(TripID const line, std::size_t from_offset, std::size_t to_offset) const
{
    // if the line isn't a line, return
    if (line == WALKING_TRANSFER || !shape_by_trip[line.base()])
        return boost::make_iterator_range(segment_table.cend(), segment_table.cend());

    auto const range = segment_table.crange(shape_by_trip[line.base()]->base());
    return boost::make_iterator_range(range.begin() + geometry_offsets[from_offset],
                                      range.begin() + geometry_offsets[to_offset] + 1);
}

} // namespace annotation
} // namespace nepomuk
