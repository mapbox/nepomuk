#include "timetable/line_table_factory.hpp"
#include "timetable/departure_table.hpp"
#include "timetable/stop_table_factory.hpp"

#include "algorithm/ranges.hpp"

#include "boost/assert.hpp"

namespace transit
{
namespace timetable
{

LineTable LineTableFactory::produce(std::vector<gtfs::StopTime>::iterator const begin,
                                    std::vector<gtfs::StopTime>::iterator const end)
{
    BOOST_ASSERT(std::distance(begin, end) != 0);
    LineTable line_table;

    auto const by_trip_id = [](auto const &lhs, auto const &rhs) {
        return lhs.trip_id < rhs.trip_id;
    };

    // use the first range of StopTime entries to create the StopTable for the route
    auto const first_trip_end = std::find_if_not(
        begin, end, [begin](auto const &stop_time) { return begin->trip_id == stop_time.trip_id; });

    // compute the stop_table
    line_table.serviced_stops = StopTableFactory::produce(begin, first_trip_end);

    using InputIter = typename std::vector<gtfs::StopTime>::iterator;
    using InputIterRange = std::pair<InputIter, InputIter>;

    // compute all departures for the line
    const auto output_inserter = [&line_table](InputIterRange const range) {
        auto const tables_are_identical = [range, &line_table]() {
            // check if the stop tables match!
            auto compare_table = StopTableFactory::produce(range.first, range.second);
            auto const &stored_table = line_table.serviced_stops;
            return compare_table == stored_table;
        };
        (void)tables_are_identical;
        BOOST_ASSERT(tables_are_identical());
        line_table.departures.departures.push_back(
            {range.first->departure, range.first->departure, 0});
    };

    algorithm::by_equal_ranges(begin, end, by_trip_id, output_inserter);

    return line_table;
}

} // namespace timetable
} // namespace transit
