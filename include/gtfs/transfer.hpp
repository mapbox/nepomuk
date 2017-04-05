#ifndef TRANSIT_GTFS_TRANSFER_HPP_
#define TRANSIT_GTFS_TRANSFER_HPP_

#include <boost/optional.hpp>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "id/stop.hpp"

namespace transit
{
namespace gtfs
{

enum class TransferType
{
    RECOMMENDED,
    TIMED,
    LONG,
    IMPOSSIBLE,
    ON_ROUTE // dedicated transfer to split cyclic routes. Not an actual transfer
};

struct Transfer
{
    StopID from;
    StopID to;

    TransferType type;
    boost::optional<std::uint64_t> minimum_duration;

    bool operator==(Transfer const &other) const;
};

bool checkTransferCSVHeader(std::map<std::string, std::size_t> const &header);
Transfer makeTransfer(std::map<std::string, std::size_t> const &header,
                      std::vector<std::string> &values);

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_TRANSFER_HPP_
