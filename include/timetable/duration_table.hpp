#ifndef TRANSIT_TIMETABLE_DURATION_TABLE_HPP_
#define TRANSIT_TIMETABLE_DURATION_TABLE_HPP_

#include <cstdint>
#include <vector>

#include <boost/range/iterator_range.hpp>

namespace transit
{
namespace timetable
{

class DurationTableFactory;

class DurationTable
{
  public:
    using const_iterator = std::vector<std::uint32_t>::const_iterator;
    using const_iterator_range = boost::iterator_range<const_iterator>;

    // list stations along the line, and their departure
    const_iterator_range list(std::size_t const offset) const;

    friend bool operator==(DurationTable const &lhs, DurationTable const &rhs);

  private:
    std::vector<std::uint32_t> arrival_delta;

    friend class DurationTableFactory;
};

bool operator==(DurationTable const &lhs, DurationTable const &rhs);

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_DURATION_TABLE_HPP_
