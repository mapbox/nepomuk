#include "gtfs/transfer.hpp"
#include "algorithm/ranges.hpp"
#include "geometric/coordinate.hpp" // for distance
#include "gtfs/stop.hpp"            // for Stop
#include "id/stop.hpp"              // for StopID
#include "timetable/exceptions.hpp"
#include "timetable/line_table.hpp"
#include "timetable/stop_table.hpp" // for StopTable
#include "timetable/transfer_table.hpp"
#include "timetable/transfer_table_factory.hpp"
#include "tool/status/progress.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream> // for operator<<, basi...
#include <iterator>
#include <set>
#include <type_traits> // for enable_if<>::type
#include <utility>     // for make_pair

namespace nepomuk
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
        std::uint32_t transfer_duration = (transfer.type == gtfs::TransferType::ON_ROUTE ||
                                           transfer.type == gtfs::TransferType::LONG)
                                              ? *transfer.minimum_duration
                                              : 0;
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

TransferTable TransferTableFactory::produce(std::vector<gtfs::Transfer>::iterator begin,
                                            std::vector<gtfs::Transfer>::iterator end,
                                            std::size_t const num_stops,
                                            std::vector<LineTable> const &line_tables,
                                            std::vector<gtfs::Stop> const &stops,
                                            search::CoordinateToStop const &stop_lookup)
{
    auto by_from = [](auto const &lhs, auto const &rhs) { return lhs.from < rhs.from; };

    std::cout << "Building new transfers..." << std::flush;
    std::vector<gtfs::Transfer> new_transfers(begin, end);

    std::vector<std::set<std::size_t>> lines(num_stops);
    for (std::size_t line = 0; line < line_tables.size(); ++line)
        for (auto stop : line_tables[line].stops().list())
        {
            lines[static_cast<std::uint64_t>(stop)].insert(line);
        }

    auto const gains_line = [&](auto const new_stop, auto const last_stop) {
        // check if we gain a new line at the stop
        for (auto line : lines[static_cast<std::uint64_t>(new_stop)])
            if (lines[static_cast<std::uint64_t>(last_stop)].count(line) == 0)
                return true;
        return false;
    };

    for (std::size_t line = 0; line < line_tables.size(); ++line)
    {
        auto stop_range = line_tables[line].stops().list();
        auto last_stop = *stop_range.begin();
        for (auto stop : stop_range)
        {
            if (gains_line(stop, last_stop))
                new_transfers.push_back({stop, stop, gtfs::TransferType::LONG, 30});
        }
    }

    { // to not clutter the rest of the scope
        std::cout << "\n[generating neighbor transfers] ";
        std::size_t current = 0;
        tool::status::ProgressBarGuard guard(stops.size());

        auto const add_close_stops_as_transfers = [&](auto const &stop) {
            // add transfers for all stops that are reachable within 50 meters aerial distance
            auto const close_stops = stop_lookup.all(stop.location, 50);
            std::for_each(
                close_stops.begin() + 1,
                close_stops.end(),
                [&new_transfers, stop, &stops](auto const neighbor) {
                    new_transfers.push_back(
                        {stop.id,
                         neighbor.first,
                         gtfs::TransferType::LONG,
                         geometric::distance(stop.location, stops[neighbor.first.base()].location) +
                             30});
                });
            guard.print(++current);
        };

        std::for_each(stops.begin(), stops.end(), add_close_stops_as_transfers);
    }

    // use stable sort to prefer original transfers
    std::stable_sort(new_transfers.begin(), new_transfers.end(), by_from);
    new_transfers.erase(std::unique(new_transfers.begin(), new_transfers.end()),
                        new_transfers.end());
    std::cout << " done (" << new_transfers.size() << " vs " << std::distance(begin, end) << ")"
              << std::endl;
    return produce(new_transfers.begin(), new_transfers.end());
}

} // namespace timetable
} // namespace nepomuk
