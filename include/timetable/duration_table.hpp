#ifndef NEPOMUK_TIMETABLE_DURATIONTABLE_HPP_
#define NEPOMUK_TIMETABLE_DURATIONTABLE_HPP_

#include <cstdint>
#include <vector>

namespace nepomuk
{
namespace timetable
{

// the global duration table contains durations for all trips in an interleafed form.
// It contains the offsets from the first departure to the arrival / departure at later stops
// e.g. s1, s2, s3, ..., sn stores durations as `a_2, d_2, ...., a_n, infty`. with `a_x` arriving at
// `x` and `d_x` departing at `x`
typedef std::vector<std::uint32_t> GlobalDurationTable;
} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_DURATIONTABLE_HPP_
