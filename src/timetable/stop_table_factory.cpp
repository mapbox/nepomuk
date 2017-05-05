#include "timetable/stop_table_factory.hpp"
#include "timetable/exceptions.hpp"

#include <algorithm>
#include <iterator>

namespace transit
{
namespace timetable
{

namespace
{
// validate the input
template <typename iterator_type> void validate_input(iterator_type begin, iterator_type end)
{
    if (std::distance(begin, end) == 0)
        throw InvalidInputError("StopTableFactory:: Stop times cannot be an empty set.");

    // trip has to be unique over all stop times
    const auto invalid_trip_id = [desired_id = begin->trip_id](auto const &stop_time)
    {
        return stop_time.trip_id != desired_id;
    };
    if (std::any_of(begin, end, invalid_trip_id))
        throw InvalidInputError("StopTableFactory:: Stop times have to belong to the same trip");

    // stop times have to be sorted by arrival
    const auto compare_by_arrival = [](auto const &lhs, auto const &rhs) {
        return lhs.arrival < rhs.arrival;
    };
    if (!std::is_sorted(begin, end, compare_by_arrival))
    {
        for (auto itr = begin; itr != end; ++itr)
            std::cout << "Station: " << itr->stop_id << " " << itr->arrival << std::endl;
        throw InvalidInputError("StopTableFactory:: Stop times need to be sorted by arrival time.");
    }
}
} // namespace

StopTable StopTableFactory::produce(std::vector<gtfs::StopTime> &stop_times)
{
    return StopTableFactory::produce(stop_times.begin(), stop_times.end());
}

StopTable StopTableFactory::produce(std::vector<gtfs::StopTime>::iterator begin,
                                    const std::vector<gtfs::StopTime>::iterator end)
{
    // make sure our input is sane
    validate_input(begin, end);

    // extract the stop-ids from the stop_times
    StopTable result;
    result.stops.reserve(std::distance(begin, end));

    // every stop is reached after X seconds, where X is the different between the arrival at the
    // stop and the departure from the very first stop.
    const auto to_stop = [](auto const &stop_time) mutable { return stop_time.stop_id; };

    std::transform(begin, end, std::back_inserter(result.stops), to_stop);
    return result;
}

} // namespace timetable
} // namespace transit
