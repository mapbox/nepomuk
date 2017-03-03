#include "timetable/timetable.hpp"
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

TimeTable TimeTableFactory::produce(gtfs::Dataset &dataset)
{
    TimeTable result;

    // ensure that trips are in correct order
    std::sort(
        dataset.stop_times.begin(), dataset.stop_times.end(), [](auto const &lhs, auto const &rhs) {
            return std::tie(lhs.trip_id, lhs.sequence_id) < std::tie(rhs.trip_id, rhs.sequence_id);
        });

    using StopTimeIter = decltype(dataset.stop_times)::iterator;
    using StopTimeIterRange = std::pair<StopTimeIter, StopTimeIter>;

    const auto stop_table_inserter = [&result](StopTimeIterRange const range) {
        result.stop_tables.push_back(StopTableFactory::produce(range.first, range.second));
    };
    const auto same_trip_id = [](gtfs::StopTime const &value, gtfs::StopTime const &candidate) {
        return value.trip_id < candidate.trip_id;
    };

    // create stop tables for all trips
    algorithm::by_equal_ranges(
        dataset.stop_times.begin(), dataset.stop_times.end(), same_trip_id, stop_table_inserter);

    return result;
}

} // namespace timetable
} // namespace transit
