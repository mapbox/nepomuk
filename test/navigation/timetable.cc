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
    auto const trip_look_up = data_service.stop_to_line();

    navigation::algorithm::TimeTable timetable_router(timetable, trip_look_up);

    auto route =
        timetable_router(date::Time("00:00:00"), StopID{0}, StopID{7});
    BOOST_CHECK((bool)route);
    BOOST_CHECK_EQUAL(
        (route->legs().begin()->segments().begin())->as_transit().stops().begin()->id(),
        StopID{0});
    BOOST_CHECK_EQUAL(
        (route->legs().begin()->segments().begin())->as_transit().connections().begin()->line(),
        LineID{0});
    route =
        timetable_router(date::Time("12:00:00"), StopID{0}, StopID{7});
    BOOST_CHECK((bool)route);
    BOOST_CHECK_EQUAL(
        (route->legs().begin()->segments().begin())->as_transit().stops().begin()->id(),
        StopID{1});

    auto leg_range = route->legs().begin()->segments();
    BOOST_CHECK_EQUAL(std::distance(leg_range.begin(), leg_range.end()), 2);
    auto route2 =
        timetable_router(date::Time("00:00:00"), StopID{0}, StopID{9});
    BOOST_CHECK((bool)route2);

    // when cyclic times routes are implemented, this will return a route again
    auto route3 =
        timetable_router(date::Time("36:00:00"), StopID{0}, StopID{7});
    BOOST_CHECK((bool)!route3);
}
