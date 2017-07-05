#include "timetable/timetable.hpp"
#include "algorithm/ranges.hpp"
#include "gtfs/dataset.hpp"
#include "gtfs/stop.hpp"
#include "gtfs/transfer.hpp" // for Transfer
#include "gtfs/trip.hpp"     // for Trip
#include "id/route.hpp"
#include "id/trip.hpp"
#include "search/coordinate_to_stop.hpp"
#include "timetable/station_table.hpp" // for StationTable
#include "timetable/station_table_factory.hpp"
#include "timetable/timetable_factory.hpp"
#include "timetable/transfer_table.hpp" // for TransferTable
#include "timetable/transfer_table_factory.hpp"
#include "timetable/trip_table_factory.hpp"

#include <algorithm>          // for stable_sort, for...
#include <boost/assert.hpp>   // for BOOST_ASSERT
#include <boost/optional.hpp> // for optional
#include <iterator>
#include <tuple>
#include <unordered_map>
#include <utility>

namespace nepomuk
{
namespace geometric
{
class WGS84Coordinate;
}
}
namespace nepomuk
{
struct ShapeID;
}
namespace nepomuk
{
struct StopID;
}

namespace nepomuk
{
struct ShapeID;
struct StopID;
namespace geometric
{
class WGS84Coordinate;
}

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

template <typename factory_type, typename input_type, typename grouping_predicate>
void produceByEqualRangesVector(std::vector<input_type> &input,
                                grouping_predicate group_by,
                                factory_type &factory)
{
    BOOST_ASSERT(input.begin() != input.end());
    // ensure correct ordering.
    const auto producer = [&](auto const range) {
        interpolate_times(range.first, range.second);
        factory.produce(range.first, range.second);
    };

    algorithm::by_equal_ranges(input.begin(), input.end(), group_by, producer);
}

} // namespace

TimeTable TimeTableFactory::produce(gtfs::Dataset &dataset,
                                    std::vector<TripID> *output_trip_mapping)
{
    TimeTable result;

    // TODO set UTC offset based on agency fields
    TripTableFactory trip_factory(0, dataset.trips);
    {
        produceByEqualRangesVector(
            dataset.stop_times,
            // process elements by their trip id
            [](gtfs::StopTime const &value, gtfs::StopTime const &candidate) {
                return value.trip_id < candidate.trip_id;
            },
            trip_factory);

        result._trip_table = trip_factory.extract();
        if (output_trip_mapping)
            *output_trip_mapping = trip_factory.get_mapping();
    }
    result.station_table = StationTableFactory::produce(dataset.stops.begin(), dataset.stops.end());

    if (dataset.transfers)
    {
        std::vector<std::pair<geometric::WGS84Coordinate, StopID>> locations;
        locations.reserve(dataset.stops.size());
        std::transform(dataset.stops.begin(),
                       dataset.stops.end(),
                       std::back_inserter(locations),
                       [](auto const &stop) { return std::make_pair(stop.location, stop.id); });
        auto const stop_lookup = search::CoordinateToStop(locations);
        // sorts internally
        result.transfer_table = TransferTableFactory::produce(dataset.transfers->begin(),
                                                              dataset.transfers->end(),
                                                              dataset.stops.size(),
                                                              result.trip_table(),
                                                              result.station_table,
                                                              dataset.stops,
                                                              stop_lookup);
    }
    return result;
}

} // namespace timetable
} // namespace nepomuk
