#include "timetable/stop_table_factory.hpp"
#include "timetable/exceptions.hpp"

#include "algorithm/ranges.hpp"

#include <boost/assert.hpp>

#include <algorithm>
#include <iterator>

namespace transit
{
namespace timetable
{

// validate the input
template <typename iterator_type> void validate_input(iterator_type begin, iterator_type end)
{
    if (std::distance(begin, end) == 0)
        throw InvalidInputError("Stop times cannot be an empty set.");

    // trip has to be unique over all stop times
    const auto invalid_trip_id = [desired_id = begin->trip_id](auto const &stop_time)
    {
        return stop_time.trip_id != desired_id;
    };
    if (std::any_of(begin, end, invalid_trip_id))
        throw InvalidInputError("Stop times have to belong to the same trip");

    // stop times have to be sorted by arrival
    const auto compare_by_arrival = [](auto const &lhs, auto const &rhs) {
        return lhs.arrival < rhs.arrival;
    };
    if (!std::is_sorted(begin, end, compare_by_arrival))
        throw InvalidInputError("Stop times need to be sorted by arrival time.");
}

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

StopTable StopTableFactory::produce(std::vector<gtfs::StopTime> &stop_times)
{
    return StopTableFactory::produce(stop_times.begin(), stop_times.end());
}

StopTable StopTableFactory::produce(std::vector<gtfs::StopTime>::iterator begin,
                                    const std::vector<gtfs::StopTime>::iterator end)
{
    interpolate_times(begin, end);
    // make sure our input is sane
    validate_input(begin, end);
    std::vector<StopTable::Arrival> stop_arrivals;
    stop_arrivals.reserve(std::distance(begin, end));

    // every stop is reached after X seconds, where X is the different between the arrival at the
    // stop and the departure from the very first stop.
    const auto to_arrival = [base_time = begin->departure](auto const &stop_time)
    {
        return StopTable::Arrival{stop_time.stop_id, stop_time.arrival - base_time};
    };

    std::transform(begin, end, std::back_inserter(stop_arrivals), to_arrival);

    // transfer the arrival times to the stop table
    StopTable result;
    result._trip_id = begin->trip_id;
    result.stop_arrivals = std::move(stop_arrivals);
    return result;
}

} // namespace timetable
} // namespace transit
