#ifndef NEPOMUK_TIMETABLE_SEGMENT_TABLE_HPP_
#define NEPOMUK_TIMETABLE_SEGMENT_TABLE_HPP_

#include "geometric/coordinate.hpp"
#include "tool/container/indexed_vector.hpp"

namespace nepomuk
{
namespace timetable
{

// A segment table contains coordinates between stops. This is useful for annotations/distance
// calculations
using SegmentTable = tool::container::IndexedVector<geometric::WGS84Coordinate>;

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_SEGMENT_TABLE_HPP_
