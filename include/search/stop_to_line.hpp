#ifndef TRANSIT_SEARCH_STOP_TO_LINE_HPP_
#define TRANSIT_SEARCH_STOP_TO_LINE_HPP_

#include <boost/range/iterator_range.hpp>
#include <vector>

#include "timetable/line_table.hpp"

#include "gtfs/stop.hpp"
#include "gtfs/trip.hpp"

namespace transit
{
namespace search
{

class StopToLineFactory;

// When performing look-ups for names / locations, we end up with a list of stop ids. The look-up
// here allows to find all lines that service a given stop to best find the next departure of a
// given line.
class StopToLine
{
  public:
    using iterator = std::vector<timetable::LineID>::const_iterator;
    using iterator_range = boost::iterator_range<iterator>;

    iterator_range operator()(gtfs::StopID stop) const;

    // the size of the look-up in number of stops
    auto size() const { return trip_table.size(); }

    friend class StopToLineFactory;

  protected:
    std::vector<std::vector<timetable::LineID>> trip_table;
};

} // namespace search
} // namespace transit

#endif // TRANSIT_SEARCH_STOP_TO_LINE_HPP_
