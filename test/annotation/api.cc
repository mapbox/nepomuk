#include "annotation/api.hpp"
#include "date/time.hpp"
#include "fixtures.h"
#include "geometric/coordinate.hpp"
#include "id/line.hpp"
#include "id/stop.hpp"
#include "navigation/algorithm/timetable.hpp"
#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "navigation/connection.hpp"
#include "navigation/leg.hpp"
#include "navigation/route.hpp"
#include "navigation/routing_algorithm.hpp"
#include "navigation/segment.hpp"
#include "navigation/stop.hpp"
#include "service/master.hpp"
#include "test/toolkit.hpp"

#include <utility>
#include <vector>

#include <boost/optional.hpp>

using namespace transit;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

struct MockNavigator : public navigation::RoutingAlgorithm
{
    MockNavigator(std::vector<timetable::LineTable> const &line_tables)
        : RoutingAlgorithm(line_tables)
    {
    }

    navigation::Route mock_trip() const
    {
        navigation::Route route;
        navigation::Leg leg;
        navigation::segment::Walk walk;
        navigation::segment::Transit transit;
        navigation::segment::Transfer transfer;
        navigation::Connection connection =
            make_connection(LineID{0}, date::UTCTimestamp(0), date::UTCTimestamp(0));
        navigation::Stop stop = make_stop(StopID{0}, date::UTCTimestamp(0), date::UTCTimestamp(0));
        set_origin(transfer, StopID{0});
        set_destination(transfer, StopID{0});

        add_stop(transit, stop);
        add_stop(transit, stop);
        add_stop(transit, stop);

        add_connection(transit, connection);
        add_connection(transit, connection);
        add_connection(transit, connection);
        add_connection(transit, connection);
        add_connection(transit, connection);

        set_departure(walk, date::UTCTimestamp(0));
        set_arrival(walk, date::UTCTimestamp(0));

        add_segment(leg, make_segment(walk));
        add_segment(leg, make_segment(transit));
        add_segment(leg, make_segment(transfer));
        add_segment(leg, make_segment(transit));
        add_segment(leg, make_segment(walk));

        add_leg(route, leg);
        add_leg(route, leg);

        return route;
    }

    boost::optional<navigation::Route>
    operator()(date::Time const, std::vector<ADLeg> const &, std::vector<ADLeg> const &) const
    {
        return {mock_trip()};
    }

    boost::optional<navigation::Route>
    operator()(date::Time const, StopID const, StopID const) const
    {
        return {mock_trip()};
    }
};

constexpr char const *const annotated_mock =
    "{\"code\":\"Ok\",\"waypoints\":[[0,0],[0,0],[0,0]],\"routes\":[{\"depart\":0,\"arrive\":0,"
    "\"duration\":0,\"summary\":\"most beautiful "
    "route\",\"legs\":[{\"depart\":0,\"arrive\":0,\"duration\":0,\"segments\":[{\"mode\":"
    "\"walking\",\"depart\":0,\"arrive\":0,\"duration\":0,\"distance\":0,\"geometry\":\"????\"},{"
    "\"mode\":\"transit\",\"depart\":0,\"arrive\":0,\"duration\":0,\"geometry\":\"g^g^\",\"stops\":"
    "[{\"name\":\"NAME HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0}],\"connections\":[{\"depart\":0,"
    "\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"}]},{\"mode\":\"transfer\",\"depart\":0,\"arrive\":0,\"stopname\":\"Stopname will be "
    "here, believe "
    "me.\",\"geometry\":\"{^g^??\",\"duration\":0},{\"mode\":\"transit\",\"depart\":0,\"arrive\":0,"
    "\"duration\":0,\"geometry\":\"g^g^\",\"stops\":[{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0}],\"connections\":[{\"depart\":0,"
    "\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"}]},{\"mode\":\"walking\",\"depart\":0,\"arrive\":0,\"duration\":0,\"distance\":0,"
    "\"geometry\":\"????\"}]},{\"depart\":0,\"arrive\":0,\"duration\":0,\"segments\":[{\"mode\":"
    "\"walking\",\"depart\":0,\"arrive\":0,\"duration\":0,\"distance\":0,\"geometry\":\"????\"},{"
    "\"mode\":\"transit\",\"depart\":0,\"arrive\":0,\"duration\":0,\"geometry\":\"g^g^\",\"stops\":"
    "[{\"name\":\"NAME HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0}],\"connections\":[{\"depart\":0,"
    "\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"}]},{\"mode\":\"transfer\",\"depart\":0,\"arrive\":0,\"stopname\":\"Stopname will be "
    "here, believe "
    "me.\",\"geometry\":\"{^g^??\",\"duration\":0},{\"mode\":\"transit\",\"depart\":0,\"arrive\":0,"
    "\"duration\":0,\"geometry\":\"g^g^\",\"stops\":[{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0},{\"name\":\"NAME "
    "HERE\",\"location\":[0,0],\"arrive\":0,\"depart\":0}],\"connections\":[{\"depart\":0,"
    "\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"},{\"depart\":0,\"arrive\":0,\"duration\":0,\"name\":\"CONNECTION NAME "
    "HERE\",\"headsign\":\"HEADSIGN "
    "HERE\"}]},{\"mode\":\"walking\",\"depart\":0,\"arrive\":0,\"duration\":0,\"distance\":0,"
    "\"geometry\":\"????\"}]}]}]}";

BOOST_AUTO_TEST_CASE(annotate_api_trip)
{
    service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    navigation::algorithm::TimeTable timetable_router(data_service.timetable(),
                                                      data_service.stop_to_line());

    annotation::API const &api = data_service.api_annotation();

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

    {
        MockNavigator mocked_navigator(data_service.timetable().lines());
        auto mocked_trip = mocked_navigator(date::Time("1:00:00"), origin, destination);
        BOOST_CHECK(mocked_trip);
        if (mocked_trip)
        {
            std::vector<navigation::Route> routes;
            routes.push_back(*mocked_trip);
            auto const annotated_trip_mock = api(routes);
            BOOST_CHECK_EQUAL(annotated_trip_mock, annotated_mock);
        }
    }
}
