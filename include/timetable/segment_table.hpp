#ifndef TRANSIT_TIMETABLE_SEGMENT_TABLE_HPP_
#define TRANSIT_TIMETABLE_SEGMENT_TABLE_HPP_

#include "geometric/coordinate.hpp"
#include "tool/container/indexed_vector.hpp"

namespace transit
{
namespace timetable
{

// A segment table contains coordinates between stops. This is useful for annotations/distance
// calculations
using SegmentTable = tool::container::IndexedVector<geometric::WGS84Coordinate>;

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_SEGMENT_TABLE_HPP_
