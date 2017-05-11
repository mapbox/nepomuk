#ifndef NEPOMUK_TIMETABLE_DURATION_TABLE_HPP_
#define NEPOMUK_TIMETABLE_DURATION_TABLE_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>

#include <boost/range/iterator_range.hpp>

namespace nepomuk
{
namespace timetable
{

class DurationTable
{
  public:
    using const_iterator = std::vector<std::uint32_t>::const_iterator;
    using const_iterator_range = boost::iterator_range<const_iterator>;

    // list stations along the line, and their departure
    std::uint32_t prefix_time(std::size_t const offset) const;
    std::uint32_t duration(std::size_t const offset) const;
    const_iterator_range list(std::size_t const offset) const;

    friend bool operator==(DurationTable const &lhs, DurationTable const &rhs);

  private:
    std::vector<std::uint32_t> arrival_delta;
    // prefix sum over all arrival times, to compute the correct departure at an offset stop in O(1)
    std::vector<std::uint32_t> arrival_prefix;

    friend class DurationTableFactory;
};

bool operator==(DurationTable const &lhs, DurationTable const &rhs);

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_DURATION_TABLE_HPP_
