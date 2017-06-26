#ifndef NEPOMUK_TIMETABLE_STOPTABLE_HPP_
#define NEPOMUK_TIMETABLE_STOPTABLE_HPP_

#include <vector>

#include "id/stop.hpp"

namespace nepomuk
{

namespace timetable
{

// The global stop table contains all unique trips in no specific order. For every line that departs
// at a stop, we can look-up the following stops along that line by providing offsets into this
// global stop table. E.g. for two lines (a,b,c,d) and (c,e,f) we would get the a global table
// containing (a,b,c,d,c,e,f).
// 
// The first line would reference (a,b,c,d,c,e,f)
//                                ^       ^ at `a`
//                                  ^     ^ at `b` and so on,
// The second line (a,b,c,d,c,e,f)
//                         ^     ^ at `c` and so on
typedef std::vector<StopID> GlobalStopTable;

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_STOPTABLE_HPP_
