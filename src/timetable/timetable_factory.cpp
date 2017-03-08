#include "timetable/timetable.hpp"
#include "timetable/departure_table_factory.hpp"
#include "timetable/stop_table_factory.hpp"
#include "timetable/timetable_factory.hpp"

#include "gtfs/stop.hpp"

#include "algorithm/ranges.hpp"

#include <tuple>
#include <utility>

namespace transit
{
namespace timetable
{

namespace
{

// template to produce items based on equal ranges in a sorted order
template <typename factory,
          typename result_type,
          typename input_type,
          typename sorting_predicate,
          typename grouping_predicate>
void produceByEqualRanges(std::vector<result_type> &output,
                          std::vector<input_type> &input,
                          sorting_predicate sort_by,
                          grouping_predicate group_by)
{
    // ensure correct ordering.
    std::stable_sort(input.begin(), input.end(), sort_by);

    using InputIter = typename std::vector<input_type>::iterator;
    using InputIterRange = std::pair<InputIter, InputIter>;

    const auto output_inserter = [&output](InputIterRange const range) {
        output.push_back(factory::produce(range.first, range.second));
    };

    algorithm::by_equal_ranges(input.begin(), input.end(), group_by, output_inserter);
}

} // namespace

TimeTable TimeTableFactory::produce(gtfs::Dataset &dataset)
{
    TimeTable result;

    produceByEqualRanges<StopTableFactory>(
        result.stop_tables,
        dataset.stop_times,
        [](auto const &lhs, auto const &rhs) {
            return std::tie(lhs.trip_id, lhs.sequence_id) < std::tie(rhs.trip_id, rhs.sequence_id);
        },
        [](gtfs::StopTime const &value, gtfs::StopTime const &candidate) {
            return value.trip_id < candidate.trip_id;
        });

    auto const by_trip_id = [](auto const &lhs, auto const &rhs) {
        return lhs.trip_id < rhs.trip_id;
    };
    if (dataset.frequencies)
        produceByEqualRanges<DepartureTableFactory>(
            result.departure_tables, *dataset.frequencies, by_trip_id, by_trip_id);

    return result;
}

} // namespace timetable
} // namespace transit
