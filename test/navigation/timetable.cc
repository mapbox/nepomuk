// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"
#include <boost/test/unit_test.hpp>

#include "id/shape.hpp"

#include "date/time.hpp"

#include "navigation/algorithm/timetable.hpp"
#include "service/master.hpp"

#include <boost/optional.hpp>
#include <iterator>
#include <vector>

#include <iostream>

using namespace nepomuk;

BOOST_AUTO_TEST_CASE(lookup_lines_from_stops)
{
    // This needs to be replaced by a dedicated fixture (see
    // https://github.com/mapbox/nepomuk/issues/37)
    service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    std::vector<boost::optional<ShapeID>> shapes_by_line;
    auto const timetable = data_service.timetable();

    navigation::algorithm::TimeTable timetable_router(timetable, data_service.stop_to_trip());

    auto route = timetable_router(date::Time("00:00:00"), StopID{0}, StopID{7});
    BOOST_CHECK((bool)route);
    BOOST_CHECK_EQUAL(
        (route->legs().begin()->segments().begin())->as_transit().stops().begin()->id(), StopID{0});
    BOOST_CHECK_EQUAL(
        (route->legs().begin()->segments().begin())->as_transit().connections().begin()->line(),
        TripID{0});
    route = timetable_router(date::Time("12:00:00"), StopID{0}, StopID{7});
    BOOST_CHECK((bool)route);
    BOOST_CHECK_EQUAL(
        (route->legs().begin()->segments().begin())->as_transit().stops().begin()->id(), StopID{1});

    auto leg_range = route->legs().begin()->segments();
    BOOST_CHECK_EQUAL(std::distance(leg_range.begin(), leg_range.end()), 2);
    auto route2 = timetable_router(date::Time("00:00:00"), StopID{0}, StopID{9});
    BOOST_CHECK((bool)route2);

    // when cyclic times routes are implemented, this will return a route again
    auto route3 = timetable_router(date::Time("36:00:00"), StopID{0}, StopID{7});
    BOOST_CHECK((bool)!route3);

    auto const &coordinate_to_stop = data_service.coordinate_to_stop();
    auto const &geometry = data_service.geometry_annotation();
    auto from = coordinate_to_stop.nearest(geometry.get(StopID{0}), 1);
    auto to = coordinate_to_stop.nearest(geometry.get(StopID{4}), 1);

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
        from.begin(), from.end(), std::back_inserter(origin), make_converter(from[0].second));
    std::transform(to.begin(),
                   to.end(),
                   std::back_inserter(destination),
                   make_converter(to[0].second));

	auto empty = origin;
    empty.clear();

    const auto route_4 = timetable_router(date::Time{"00:00:00"}, origin, destination);
    BOOST_CHECK((bool)route_4);
    const auto route_5 = timetable_router(date::Time{"00:00:00"}, origin, empty);
    BOOST_CHECK(!(bool)route_5);
    const auto route_6 = timetable_router(date::Time{"00:00:00"}, empty, empty);
    BOOST_CHECK(!(bool)route_6);
    const auto route_7 = timetable_router(date::Time{"00:00:00"}, empty, destination);
    BOOST_CHECK(!(bool)route_7);
}
