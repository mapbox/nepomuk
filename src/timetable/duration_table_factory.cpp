#include "timetable/duration_table_factory.hpp"
#include "timetable/exceptions.hpp"

#include "algorithm/ranges.hpp"

namespace transit
{
namespace timetable
{
namespace
{
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
} // namespace

DurationTable DurationTableFactory::produce(std::vector<gtfs::StopTime>::iterator begin,
                                            const std::vector<gtfs::StopTime>::iterator end)
{
    // make sure our input is fine
    validate_input(begin, end);

    DurationTable result;
    result.arrival_delta.reserve(std::distance(begin, end));
    result.arrival_prefix.reserve(std::distance(begin, end));

    // every stop is reached after X seconds, where X is
    // the different between the arrival at the
    // stop and the departure from the very first stop.
    const auto to_delta = [ base_time = begin->departure, route_start = begin->departure, &result ](
        auto const &stop_time) mutable
    {
        result.arrival_prefix.push_back(stop_time.arrival - route_start);
        auto result = stop_time.arrival - base_time;
        base_time = stop_time.arrival;
        return result;
    };

    std::transform(begin, end, std::back_inserter(result.arrival_delta), to_delta);

    return result;
}

} // namespace timetable
} // namespace transit
