#include "annotation/api.hpp"
#include "date/time.hpp"
#include "fixtures.h"
#include "geometric/coordinate.hpp"
#include "id/line.hpp"
#include "id/stop.hpp"
#include "navigation/algorithm/timetable.hpp"
#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "navigation/route.hpp"
#include "service/master.hpp"
#include "test/toolkit.hpp"

#include <utility>
#include <vector>

using namespace transit;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(annotate_api_trip)
{
    service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    navigation::algorithm::TimeTable timetable_router(data_service.timetable(),
                                                      data_service.stop_to_line());

    annotation::API api(data_service.geometry_annotation());

    auto const &coordinate_to_stop = data_service.coordinate_to_stop();

    auto const from_coordinate = make_coordinate(0.005, 0.005);
    auto const to_coordinate = make_coordinate(0.0556, 0.20601);
    auto const from = coordinate_to_stop.all(make_coordinate(0.005, 0.005), 20);
    auto const to = coordinate_to_stop.all(make_coordinate(0.0556, 0.20601), 20);

    using input_type = navigation::RoutingAlgorithm::ADLeg;
    std::vector<input_type> origin;
    std::vector<input_type> destination;

    auto const make_converter = [](auto const base_coordinate) {
        return [base_coordinate](auto const &pair) -> input_type {
            auto const distance = geometric::distance(base_coordinate, pair.second);
            return {pair.first, static_cast<std::uint32_t>(std::llround(2 * distance)), distance};
        };
    };

    std::transform(
        from.begin(), from.end(), std::back_inserter(origin), make_converter(from_coordinate));
    std::transform(
        to.begin(), to.end(), std::back_inserter(destination), make_converter(to_coordinate));

    auto trip = timetable_router(date::Time("1:00:00"), origin, destination);
    BOOST_CHECK(static_cast<bool>(trip));

    if (trip)
    {
        std::vector<navigation::Route> routes;
        routes.push_back(*trip);
        auto const annotated_trip = api(routes);
        BOOST_CHECK_EQUAL(API_ANNOTATED_QUERY_A_J, annotated_trip);
    }
}
