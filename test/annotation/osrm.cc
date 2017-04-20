#include "date/time.hpp"
#include "fixtures.h"
#include "geometric/coordinate.hpp"
#include "id/line.hpp"
#include "id/stop.hpp"
#include "navigation/algorithm/timetable.hpp"
#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "service/master.hpp"
#include "test/toolkit.hpp"

#include <utility>
#include <vector>

using namespace transit;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(annotate_osrm_trip)
{
    service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    navigation::algorithm::TimeTable timetable_router(data_service.timetable(),
                                                      data_service.stop_to_line());

    auto const &osrm_annotation = data_service.osrm_annotation();
    BOOST_CHECK_EQUAL(&osrm_annotation, &data_service.osrm_annotation());

    auto trip = timetable_router(date::Time("1:00:00"), StopID{1}, StopID{4});

    auto annotated_trip = osrm_annotation(*trip);

    BOOST_CHECK(static_cast<bool>(trip));

    BOOST_CHECK_EQUAL(OSRM_ANNOTATED_QUERY_1_4, annotated_trip);
}

BOOST_AUTO_TEST_CASE(annotate_osrm_trip_from_coordinates)
{
    service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    navigation::algorithm::TimeTable timetable_router(data_service.timetable(),
                                                      data_service.stop_to_line());
    navigation::algorithm::TimeTableDijkstra timetable_dijkstra(data_service.timetable(),
                                                        data_service.stop_to_line());

    auto const coordinate_to_stop = data_service.coordinate_to_stop();

    auto const get_locations = [&](auto const from, double radius) {
        auto result = coordinate_to_stop.all(from, radius);
        while (result.empty() && radius < 1000)
        {
            radius *= 2;
            result = coordinate_to_stop.all(from, radius);
        }
        return result;
    };

    auto const source = make_coordinate(0.0049,0.005);
    auto const target = make_coordinate(0.0354,0.005);

    auto const from = get_locations(source, 50);
    auto const to = get_locations(target, 50);

    using input_type = navigation::RoutingAlgorithm::ADLeg;
    std::vector<input_type> origin;
    std::vector<input_type> destination;
    auto const empty = timetable_router(date::Time("1:00:00"), origin, destination);
    BOOST_CHECK(!empty);
    auto const empty_dijkstra = timetable_dijkstra(date::Time("1:00:00"), origin, destination);
    BOOST_CHECK(!empty_dijkstra);

    auto const make_converter = [](auto const base_coordinate) {
        return [base_coordinate](auto const &pair) -> input_type {
            auto const distance = geometric::distance(base_coordinate, pair.second);
            return {pair.first, static_cast<std::uint32_t>(std::llround(2 * distance)), distance};
        };
    };

    std::transform(from.begin(),
                   from.end(),
                   std::back_inserter(origin),
                   make_converter(source));
    std::transform(to.begin(),
                   to.end(),
                   std::back_inserter(destination),
                   make_converter(target));

    auto const &osrm_annotation = data_service.osrm_annotation();
    BOOST_CHECK_EQUAL(&osrm_annotation, &data_service.osrm_annotation());

    auto trip = timetable_router(date::Time("1:00:00"), origin, destination);
    auto dtrip = timetable_dijkstra(date::Time("1:00:00"), origin, destination);

    auto annotated_trip = osrm_annotation(*trip);
    auto annotated_dtrip = osrm_annotation(*dtrip);

    BOOST_CHECK(static_cast<bool>(trip));
    BOOST_CHECK(static_cast<bool>(dtrip));

    BOOST_CHECK_EQUAL(OSRM_ANNOTATED_QUERY_1_4, annotated_trip);
    BOOST_CHECK_EQUAL(OSRM_ANNOTATED_QUERY_1_4, annotated_dtrip);
}
