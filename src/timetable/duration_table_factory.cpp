#include "timetable/duration_table_factory.hpp"
#include "timetable/exceptions.hpp"

#include "date/time.hpp" // for Time, operator-
#include <algorithm>     // for transform, any_of
#include <cstdint>       // for uint32_t
#include <iterator>      // for back_insert_iter...

namespace nepomuk
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
    DurationTable result;

    // make sure our input is fine
    validate_input(begin, end);

    result.arrival_delta.resize(std::distance(begin, end));
    result.arrival_prefix.resize(std::distance(begin, end));

    // every stop is reached after X seconds, where X is
    // the different between the arrival at the
    // stop and the departure from the very first stop.

    // this code would be nicer as a transform, it triggers an asan error though, if we use
    // push_back or std::transform here
    auto const route_start = begin->departure;
    auto last_arrival = begin->departure;
    std::size_t index = 0;
    for (auto itr = begin; itr != end; ++itr)
    {
        result.arrival_prefix[index] = itr->arrival - route_start;
        result.arrival_delta[index] = itr->arrival - last_arrival;
        last_arrival = itr->arrival;
        ++index;
    }
    return result;
}

} // namespace timetable
} // namespace nepomuk
