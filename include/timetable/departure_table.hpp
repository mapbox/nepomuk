#ifndef TRANSIT_TIMETABLE_DEPARTURETABLE_HPP_
#define TRANSIT_TIMETABLE_DEPARTURETABLE_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>

#include <boost/range/iterator_range.hpp>

#include "date/time.hpp"

namespace transit
{
namespace timetable
{

// Public transit routing consists of timetable routing. The departuretables define when connections
// leave certain stops. In general these occurr in certain periodic settings. For any time at any
// stop we need to be able to access the next connections that are possible. The departuretable
// itself assumes the stop, line, and the time of arrival are known and allows  quick access to the
// next connection of the given departure. It only represents a single line.
class DepartureTable
{
  public:
    struct Departure
    {
        date::Time begin;
        date::Time end;
        std::uint32_t headway;
        std::size_t duration_table_index;

        bool operator<(Departure const &other) const;

        date::Time get_next_departure(date::Time const starting_at) const;
    };

    // iterator types
    using const_iterator = std::vector<Departure>::const_iterator;
    using const_iterator_range = boost::iterator_range<const_iterator>;

    // access for construction
    friend class DepartureTableFactory;
    friend class LineTableFactory;

    // list all valid departures from a given time
    const_iterator_range list(date::Time starting_at) const;

  private:
    std::vector<Departure> departures;
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_DEPARTURETABLE_HPP_
