#include "gtfs/transfer.hpp"
#include "gtfs/constructor.hpp"

#include <boost/assert.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/optional.hpp>

#include <tuple>

namespace transit
{
namespace gtfs
{

bool checkTransferCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("from_stop_id") && header.count("to_stop_id") &&
           header.count("transfer_type");
}

Transfer makeTransfer(std::map<std::string, std::size_t> const &header,
                      std::vector<std::string> &values)
{
    auto const to_type = [](std::string const &value) {
        if (value == "1")
            return TransferType::TIMED;
        else if (value == "2")
            return TransferType::LONG;
        else if (value == "3")
            return TransferType::IMPOSSIBLE;
        else
        {
            BOOST_ASSERT(value.empty() || value == "0");
            return TransferType::RECOMMENDED;
        }
    };
    return {
        construct<StopID>("from_stop_id", stringToID<StopID>, header, values),
        construct<StopID>("to_stop_id", stringToID<StopID>, header, values),
        to_type(construct<std::string>("transfer_type", forward, header, values)),
        construct_as_optional<std::uint64_t, false>("min_transfer_time", toInt, header, values)};
}

bool Transfer::operator==(Transfer const &other) const
{
    return std::tie(from, to, type, minimum_duration) ==
           std::tie(other.from, other.to, other.type, other.minimum_duration);
}

} // namespace gtfs
} // namespace transit
