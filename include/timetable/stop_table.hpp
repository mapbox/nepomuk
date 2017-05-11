#ifndef NEPOMUK_TIMETABLE_STOPTABLE_HPP_
#define NEPOMUK_TIMETABLE_STOPTABLE_HPP_

#include <cstdint>
#include <vector>

#include "id/stop.hpp"

#include <boost/range/iterator_range.hpp>

namespace nepomuk
{

namespace search
{
class StopToLineFactory;
} // namespace search

namespace timetable
{

// For any given line, the stop table defines the list of stops that can be reached along the line
// and the time from a given departure that is required to do so. It groups all entries that follow
// the same stopping pattern. If patterns diverge, the table requires two different instantiations.
class StopTable
{
  public:
    using const_iterator = std::vector<StopID>::const_iterator;
    using const_iterator_range = boost::iterator_range<const_iterator>;

    // list stations along the line, and their departure
    const_iterator_range list(StopID const start_id) const;
    const_iterator_range list() const;

    bool operator==(StopTable const &other) const;

  private:
    std::vector<StopID> stops;

    friend class StopTableFactory;
    friend class nepomuk::search::StopToLineFactory;
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_STOPTABLE_HPP_
