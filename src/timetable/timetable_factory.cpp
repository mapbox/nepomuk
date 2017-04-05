#include "timetable/timetable.hpp"
#include "timetable/line_table_factory.hpp"
#include "timetable/station_table_factory.hpp"
#include "timetable/timetable_factory.hpp"
#include "timetable/transfer_table_factory.hpp"

#include "id/stop.hpp"
#include "id/trip.hpp"
#include "id/route.hpp"
#include "gtfs/stop.hpp"

#include "algorithm/ranges.hpp"

#include <tuple>
#include <unordered_map>
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
    BOOST_ASSERT(input.begin() != input.end());
    // ensure correct ordering.
    std::stable_sort(input.begin(), input.end(), sort_by);

    using InputIter = typename std::vector<input_type>::iterator;
    using InputIterRange = std::pair<InputIter, InputIter>;

    const auto output_inserter = [&output](InputIterRange const range) {
        output.push_back(factory::produce(range.first, range.second));
    };

    algorithm::by_equal_ranges(input.begin(), input.end(), group_by, output_inserter);
}

template <typename factory,
          typename result_type,
          typename input_type,
          typename sorting_predicate,
          typename grouping_predicate>
void produceByEqualRangesVector(std::vector<result_type> &output,
                                std::vector<input_type> &input,
                                sorting_predicate sort_by,
                                grouping_predicate group_by)
{
    BOOST_ASSERT(input.begin() != input.end());
    // ensure correct ordering.
    std::stable_sort(input.begin(), input.end(), sort_by);

    using InputIter = typename std::vector<input_type>::iterator;
    using InputIterRange = std::pair<InputIter, InputIter>;

    const auto output_inserter = [&output](InputIterRange const range) {
        auto result_range = factory::produce(range.first, range.second);
        for (auto itr = result_range.begin(); itr != result_range.end(); ++itr)
            output.push_back(std::move(*itr));
    };

    algorithm::by_equal_ranges(input.begin(), input.end(), group_by, output_inserter);
}

template <typename factory_type,
          typename result_type,
          typename input_type,
          typename sorting_predicate,
          typename grouping_predicate>
void produceByEqualRangesVector(std::vector<result_type> &output,
                                std::vector<input_type> &input,
                                sorting_predicate sort_by,
                                grouping_predicate group_by,
                                factory_type &factory)
{
    BOOST_ASSERT(input.begin() != input.end());
    // ensure correct ordering.
    std::stable_sort(input.begin(), input.end(), sort_by);

    using InputIter = typename std::vector<input_type>::iterator;
    using InputIterRange = std::pair<InputIter, InputIter>;

    const auto output_inserter = [&output, &factory](InputIterRange const range) {
        auto result_range = factory.produce(range.first, range.second);
        for (auto itr = result_range.begin(); itr != result_range.end(); ++itr)
            output.push_back(std::move(*itr));
    };

    algorithm::by_equal_ranges(input.begin(), input.end(), group_by, output_inserter);
}

} // namespace

TimeTable TimeTableFactory::produce(gtfs::Dataset &dataset)
{
    TimeTable result;

    // prepare routes for trips
    std::unordered_map<TripID, RouteID> route_id_by_trip;
    auto const add_trip_route_mapping = [&route_id_by_trip](auto const &trip) {
        route_id_by_trip[trip.id] = trip.route_id;
    };
    std::for_each(dataset.trips.begin(), dataset.trips.end(), add_trip_route_mapping);

    {
        if (!dataset.transfers)
            dataset.transfers = std::vector<gtfs::Transfer>();

        LineTableFactory factory(*dataset.transfers);
        produceByEqualRangesVector(
            result.line_tables,
            dataset.stop_times,
            [&route_id_by_trip](auto const &lhs, auto const &rhs) {
                // group all trips by their route-id, their trip id, their sequence id
                return std::tie(route_id_by_trip.find(lhs.trip_id)->second,
                                lhs.trip_id,
                                lhs.sequence_id) <
                       std::tie(route_id_by_trip.find(rhs.trip_id)->second,
                                rhs.trip_id,
                                rhs.sequence_id);
            },
            // process elements by their trip id
            [&route_id_by_trip](gtfs::StopTime const &value, gtfs::StopTime const &candidate) {
                return route_id_by_trip.find(value.trip_id)->second <
                       route_id_by_trip.find(candidate.trip_id)->second;
            },
            factory);
    }

    result.station_table = StationTableFactory::produce(dataset.stops.begin(), dataset.stops.end());

    if (dataset.transfers)
    {
        /*
        std::sort(dataset.transfers->begin(),
                  dataset.transfers->end(),
                  [](auto const &lhs, auto const &rhs) { return lhs.from < rhs.from; });
        result.transfer_table =
            TransferTableFactory::produce(dataset.transfers->begin(), dataset.transfers->end());
        */
        // sorts internally
        result.transfer_table = TransferTableFactory::produce(dataset.transfers->begin(),
                                                              dataset.transfers->end(),
                                                              dataset.stops.size(),
                                                              result.line_tables);
    }

    return result;
}

} // namespace timetable
} // namespace transit
