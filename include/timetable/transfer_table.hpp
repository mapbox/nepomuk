#ifndef TRANSIT_TIMETABLE_TRANSFER_TABLE_HPP_
#define TRANSIT_TIMETABLE_TRANSFER_TABLE_HPP_

#include "id/stop.hpp"
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/range/iterator_range.hpp>

namespace transit
{
namespace timetable
{

class TransferTableFactory;

class TransferTable
{
  public:
    struct Transfer
    {
        StopID stop_id;
        std::uint32_t duration;
    };

    using iterator = std::vector<Transfer>::const_iterator;
    using iterator_range = boost::iterator_range<iterator>;

    iterator_range get(StopID const stop_id) const;

  private:
    friend class TransferTableFactory;

    std::unordered_map<StopID, std::pair<std::size_t, std::size_t>> index;
    std::vector<Transfer> transfers;
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_TRANSFER_TABLE_HPP_
