#include "timetable/line_table_factory.hpp"
#include "algorithm/ranges.hpp"
#include "date/time.hpp"     // for operator-, Time
#include "gtfs/transfer.hpp" // for Transfer, Transf...
#include "gtfs/trip.hpp"     // for Trip
#include "id/shape.hpp"      // for ShapeID
#include "id/stop.hpp"       // for StopID
#include "id/trip.hpp"       // for TripID
#include "timetable/departure_table.hpp"
#include "timetable/duration_table.hpp" // for DurationTable
#include "timetable/duration_table_factory.hpp"
#include "timetable/line_table.hpp" // for LineTable
#include "timetable/stop_table.hpp" // for StopTable, StopT...
#include "timetable/stop_table_factory.hpp"

#include <algorithm>
#include <boost/assert.hpp>
#include <cstddef>  // for size_t
#include <cstdint>  // for uint32_t
#include <iterator> // for distance
#include <set>
#include <utility> // for pair

namespace nepomuk
{
namespace timetable
{

namespace
{
template <typename iterator_type>
void interpolate_times(iterator_type begin, const iterator_type end)
{
    if (std::none_of(
            begin, end, [](auto const &stop_time) { return stop_time.arrival.interpolated; }))
        return;

    BOOST_ASSERT(!begin->arrival.interpolated);
    BOOST_ASSERT(!(end - 1)->arrival.interpolated);

    auto const interpolate = [](std::pair<iterator_type, iterator_type> range) {
        const auto delta_t_seconds_total = range.second->arrival - (range.first - 1)->arrival;
        const auto delta_t_seconds =
            delta_t_seconds_total / (std::distance(range.first, range.second) + 1);
        auto current_time = (range.first - 1)->arrival;
        auto const set_new_time = [&current_time, delta_t_seconds](auto stop_time) {
            current_time = current_time + delta_t_seconds;
            stop_time.arrival = current_time;
            return stop_time;
        };
        // in place transform
        std::transform(range.first, range.second, range.first, set_new_time);
    };

    // interpolate between two non-interpolated times
    algorithm::on_ranges(begin,
                         end,
                         [](const auto &stop_time) { return stop_time.arrival.interpolated; },
                         interpolate);
}

} // namspace

LineTableFactory::LineTableFactory(std::vector<gtfs::Transfer> &transfers,
                                   std::vector<std::size_t> &trip_offsets_by_line)
    : transfers(transfers), trip_offsets_by_line(trip_offsets_by_line)
{
}

std::vector<LineTable> LineTableFactory::produce(std::vector<gtfs::StopTime>::iterator const begin,
                                                 std::vector<gtfs::StopTime>::iterator const end)
{
    BOOST_ASSERT(std::distance(begin, end) != 0);
    std::vector<LineTable> line_tables;

    auto const by_trip_id = [](auto const &lhs, auto const &rhs) {
        return lhs.trip_id < rhs.trip_id;
    };

    using InputIter = typename std::vector<gtfs::StopTime>::iterator;
    using InputIterRange = std::pair<InputIter, InputIter>;

    // compute all departures for the line
    const auto output_inserter = [&line_tables, this](InputIterRange const range) {
        interpolate_times(range.first, range.second);

        auto processed_until = range.first;
        while (processed_until != range.second)
        {

            std::set<StopID> distinct_stops;
            auto const add_stops_until_not_distinct = [&distinct_stops](auto const &stop_time) {
                auto non_distinct = distinct_stops.count(stop_time.stop_id) > 0;
                distinct_stops.insert(stop_time.stop_id);
                return non_distinct;
            };
            auto distinct_end =
                std::find_if(processed_until, range.second, add_stops_until_not_distinct);

            auto stop_table = StopTableFactory::produce(processed_until, distinct_end);

            if (distinct_end != range.second)
            {
                // requires a new transfer to stay in the vehicle:
                gtfs::Transfer transfer{(distinct_end - 1)->stop_id,
                                        distinct_end->stop_id,
                                        gtfs::TransferType::ON_ROUTE,
                                        distinct_end->arrival - (distinct_end - 1)->arrival};
                transfers.push_back(std::move(transfer));
            }

            auto const line_index = [this, &range, &line_tables, stop_table]() {
                auto const itr =
                    std::find_if(line_tables.begin(), line_tables.end(), [&](auto const &line) {
                        return line.stop_table == stop_table;
                    });

                if (itr == line_tables.end())
                {
                    line_tables.push_back(LineTable());
                    // remember the trips shape id for later
                    trip_offsets_by_line.push_back(range.first->trip_id.base());
                    line_tables.back().stop_table = stop_table;
                    line_tables.back().time_deltas.resize(stop_table.list().size(), 0);
                    return line_tables.size() - 1;
                }
                else
                {
                    return static_cast<std::size_t>(std::distance(line_tables.begin(), itr));
                }
            }();

            auto duration_table = DurationTableFactory::produce(processed_until, distinct_end);
            auto const duration_index = [&line_tables, &duration_table, line_index]() {
                auto const itr = std::find(line_tables[line_index].duration_tables.begin(),
                                           line_tables[line_index].duration_tables.end(),
                                           duration_table);
                if (itr == line_tables[line_index].duration_tables.end())
                {
                    line_tables[line_index].duration_tables.push_back(duration_table);
                    return line_tables[line_index].duration_tables.size() - 1;
                }
                else
                {
                    return static_cast<std::size_t>(
                        std::distance(line_tables[line_index].duration_tables.begin(), itr));
                }
            }();

            BOOST_ASSERT(processed_until != range.second);
            line_tables[line_index].departures.departures.push_back(
                {processed_until->departure, processed_until->departure, 0, duration_index});

            // remember the maximum times for a segment
            for (std::size_t time_index = 0;
                 time_index < line_tables[line_index].time_deltas.size();
                 ++time_index)
                line_tables[line_index].time_deltas[time_index] =
                    std::max(line_tables[line_index].time_deltas[time_index],
                             duration_table.duration(time_index));

            processed_until = distinct_end;
        }

    };

    algorithm::by_equal_ranges(begin, end, by_trip_id, output_inserter);

    // sort departures
    for (auto &line_table : line_tables)
    {
        std::sort(line_table.departures.departures.begin(),
                  line_table.departures.departures.end(),
                  [](auto const &lhs, auto const &rhs) { return lhs.begin < rhs.begin; });
        // turn maximum values into prefix sum
        auto const to_prefix_sum = [sum = std::size_t{0}](auto const delta) mutable
        {
            sum += delta;
            return sum;
        };
        std::transform(line_table.time_deltas.begin(),
                       line_table.time_deltas.end(),
                       line_table.time_deltas.begin(),
                       to_prefix_sum);
    }

    return line_tables;
}

} // namespace timetable
} // namespace nepomuk
