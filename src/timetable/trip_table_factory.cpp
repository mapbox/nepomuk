#include "timetable/trip_table_factory.hpp"
#include "timetable/exceptions.hpp"

#include <algorithm>
#include <iterator> // for back_insert_iter...

#include <boost/numeric/conversion/cast.hpp>

namespace nepomuk
{
namespace timetable
{

namespace
{

// validate the input
template <typename iterator_type> void validate_input_stops(iterator_type begin, iterator_type end)
{
    if (std::distance(begin, end) <= 1)
        throw InvalidInputError("TripTableFacotry::Stops Stop times cannot be an empty set.");

    // trip has to be unique over all stop times
    const auto invalid_trip_id = [desired_id = begin->trip_id](auto const &stop_time)
    {
        return stop_time.trip_id != desired_id;
    };
    if (std::any_of(begin, end, invalid_trip_id))
        throw InvalidInputError(
            "TripTableFactory::Stops Stop times have to belong to the same trip");

    // stop times have to be sorted by arrival
    const auto compare_by_arrival = [](auto const &lhs, auto const &rhs) {
        return lhs.arrival < rhs.arrival;
    };
    if (!std::is_sorted(begin, end, compare_by_arrival))
    {
        for (auto itr = begin; itr != end; ++itr)
            std::cout << "Station: " << itr->stop_id << " " << itr->arrival << std::endl;
        throw InvalidInputError(
            "TripTableFactory::Stops Stop times need to be sorted by arrival time.");
    }
}

template <typename iterator_type>
void validate_input_durations(iterator_type begin, iterator_type end)
{
    if (std::distance(begin, end) <= 1)
        throw InvalidInputError("TripTableFactory::Durations Stop times cannot be an empty set.");

    // trip has to be unique over all stop times
    const auto invalid_trip_id = [desired_id = begin->trip_id](auto const &stop_time)
    {
        return stop_time.trip_id != desired_id;
    };
    if (std::any_of(begin, end, invalid_trip_id))
        throw InvalidInputError(
            "TripTableFactory::Durations Stop times have to belong to the same trip");

    // stop times have to be sorted by arrival
    const auto compare_by_arrival = [](auto const &lhs, auto const &rhs) {
        return lhs.arrival < rhs.arrival;
    };
    if (!std::is_sorted(begin, end, compare_by_arrival))
        throw InvalidInputError(
            "TripTableFactory::Durations Stop times need to be sorted by arrival time.");
}

} // namespace

//
// Handle Stops
//
std::size_t TripTableFactory::produceStopOffset(std::vector<gtfs::StopTime>::iterator begin,
                                                const std::vector<gtfs::StopTime>::iterator end)
{
    // make sure our input is sane
    validate_input_stops(begin, end);

    auto existing_table = findStopOffset(begin, end);
    if (existing_table)
        return *existing_table;

    // starting at the end of all stops
    auto from = table.all_stops.size();
    // remember the current size, since stop_id can be foudn here
    stop_locations_stops[begin->stop_id].push_back(from);

    // every stop is reached after X seconds, where X is the different between the arrival at the
    // stop and the departure from the very first stop.
    const auto to_stop = [](auto const &stop_time) { return stop_time.stop_id; };
    std::transform(begin, end, std::back_inserter(table.all_stops), to_stop);

    // mark the end of a stop_range
    table.all_stops.push_back(StopID::INVALID());

    return from;
}

boost::optional<std::size_t>
TripTableFactory::findStopOffset(const std::vector<gtfs::StopTime>::iterator begin,
                                 const std::vector<gtfs::StopTime>::iterator end)
{
    auto const &locations = stop_locations_stops[begin->stop_id];
    auto const distance = std::distance(begin, end);

    // check if the stops at offset match the stop range
    auto const same_stops = [&](auto const offset) {
        auto const length = std::distance(begin, end);
        if (offset + length >= table.all_stops.size())
            return false;

        auto const has_same_id = [](auto const stop, auto const &stop_time) {
            return stop == stop_time.stop_id;
        };

        auto const same_length =
            *(table.all_stops.begin() + offset + distance) == StopID::INVALID();

        return same_length && std::equal(table.all_stops.begin() + offset,
                                         table.all_stops.begin() + offset + distance,
                                         begin,
                                         has_same_id);
    };

    auto const itr = std::find_if(locations.begin(), locations.end(), same_stops);

    if (itr != locations.end())
    {
        return {*itr};
    }
    else
        return boost::none;
}

//
// Duration Handling
//

std::size_t TripTableFactory::produceDurationOffset(std::vector<gtfs::StopTime>::iterator begin,
                                                    const std::vector<gtfs::StopTime>::iterator end)
{
    // make sure our input is fine
    validate_input_durations(begin, end);

    std::vector<std::uint32_t> as_durations;
    auto const add_offsets = [&](auto const &stop_time) {
        as_durations.push_back(stop_time.arrival - begin->departure);
        as_durations.push_back(stop_time.departure - begin->departure);
    };

    BOOST_ASSERT(std::is_sorted(as_durations.begin(), as_durations.end()));

    // sentinel for the last departure
    std::for_each(begin, end, add_offsets);
    as_durations.front() = as_durations.back() = std::numeric_limits<std::uint32_t>::max();

    auto existing_table = findDurationOffset(as_durations.begin(), as_durations.end());
    if (existing_table)
        return *existing_table;

    // starting at the end of all stops
    auto from = table.all_durations.size();

    // remember the current size, since stop_id can be foudn here
    BOOST_ASSERT(as_durations.size() > 2);
    locations_durations[*(as_durations.begin() + 2)].push_back(from);

    // mark the end of a stop_range
    table.all_durations.insert(table.all_durations.end(), as_durations.begin(), as_durations.end());

    return from;
}

boost::optional<std::size_t>
TripTableFactory::findDurationOffset(const std::vector<std::uint32_t>::iterator begin,
                                     const std::vector<std::uint32_t>::iterator end)
{
    // select first non-zero duration
    BOOST_ASSERT(std::distance(begin, end) > 3);
    auto const &locations = locations_durations[*(begin + 2)];
    auto const distance = std::distance(begin, end);

    // check if the durations at offset match the stop range
    auto const same_durations = [&](auto const offset) {
        auto const length = std::distance(begin, end);
        if (offset + length > table.all_durations.size())
            return false;

        return std::equal(table.all_durations.begin() + offset,
                          table.all_durations.begin() + offset + distance,
                          begin);
    };

    auto const itr = std::find_if(locations.begin(), locations.end(), same_durations);

    if (itr != locations.end())
    {
        return {*itr};
    }
    else
        return boost::none;
}

//
// Overall handlings
//
TripTableFactory::TripTableFactory(std::int32_t utc_offset) : utc_offset(utc_offset) {}

TripID TripTableFactory::produce(std::vector<gtfs::StopTime>::iterator begin,
                                 std::vector<gtfs::StopTime>::iterator end)
{
    // at some time frequency / stop time might need different checks, right now we are fine doing
    // it this way and miss-using the input validity template

    // find a position of stops
    auto const stop_offset = produceStopOffset(begin, end);
    auto const duration_offset = produceDurationOffset(begin, end);

    // the current departure gets its ID
    TripID id = {table.departures.size()};

    // remember external trip_ids to ensure that we can access geometry later on
    external_from_internal_trip_id.push_back(begin->trip_id);
    table.departures.push_back({begin->departure + utc_offset, stop_offset, duration_offset});
    return id;
}

} // namespace timetable
} // namespace nepomuk
