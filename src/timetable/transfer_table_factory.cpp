#include "gtfs/transfer.hpp"
#include "algorithm/ranges.hpp"
#include "timetable/exceptions.hpp"
#include "timetable/transfer_table.hpp"
#include "timetable/transfer_table_factory.hpp"

#include <algorithm>
#include <iterator>

namespace transit
{
namespace timetable
{

namespace
{
template <typename iterator_type>
void validate_input(iterator_type const begin, iterator_type const end)
{
    if (std::distance(begin, end) <= 0)
        throw InvalidInputError("List of transfers cannot be empty");

    // a transfer that specifies MinLength without giving the min length
    auto const type_is_invalid = [](auto const &transfer) {
        return transfer.type == gtfs::TransferType::LONG && !transfer.minimum_duration;
    };

    if (std::find_if(begin, end, type_is_invalid) != end)
        throw InvalidInputError("Not all transfer types are valid");

    if (!std::is_sorted(
            begin, end, [](auto const &lhs, auto const &rhs) { return lhs.from < rhs.from; }))
        throw InvalidInputError("Transfers need to be sorted by origin of transfer");
}
} // namespace

TransferTable TransferTableFactory::produce(std::vector<gtfs::Transfer>::iterator begin,
                                            std::vector<gtfs::Transfer>::iterator end)
{
    // make sure transfers are sorted
    validate_input(begin, end);

    TransferTable result;

    auto by_from = [](auto const &lhs, auto const &rhs) { return lhs.from < rhs.from; };

    auto to_transfer = [](auto const &transfer) {
        std::uint32_t transfer_duration =
            transfer.type == gtfs::TransferType::LONG ? *transfer.minimum_duration : 0;
        return TransferTable::Transfer{transfer.to, transfer_duration};
    };

    auto add_transfer_block = [&result, begin, to_transfer](auto const iterator_range) {
        result.index[iterator_range.first->from] = std::make_pair(
            result.transfers.size(),
            result.transfers.size() + std::distance(iterator_range.first, iterator_range.second));

        std::transform(iterator_range.first,
                       iterator_range.second,
                       std::back_inserter(result.transfers),
                       to_transfer);
    };

    algorithm::by_equal_ranges(begin, end, by_from, add_transfer_block);

    return result;
}

} // namespace timetable
} // namespace transit
