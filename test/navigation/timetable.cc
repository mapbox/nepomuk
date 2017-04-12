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

BOOST_AUTO_TEST_CASE(lookup_lines_from_stops)
{
    // This needs to be replaced by a dedicated fixture (see
    // https://github.com/mapbox/directions-transit/issues/37)
    transit::service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    std::vector<boost::optional<transit::ShapeID>> shapes_by_line;
    auto const timetable = data_service.timetable();
    auto const trip_look_up = data_service.stop_to_line();

    transit::navigation::algorithm::TimeTable timetable_router(timetable, trip_look_up);

    auto route =
        timetable_router(transit::date::Time("00:00:00"), transit::StopID{0}, transit::StopID{7});
    BOOST_CHECK((bool)route);
    BOOST_CHECK_EQUAL(route->list().begin()->list().begin()->stop_id, transit::StopID{0});
    BOOST_CHECK_EQUAL(route->list().begin()->line(), transit::LineID{0});
    route =
        timetable_router(transit::date::Time("12:00:00"), transit::StopID{0}, transit::StopID{7});
    BOOST_CHECK((bool)route);
    BOOST_CHECK_EQUAL(route->list().begin()->list().begin()->stop_id, transit::StopID{1});

    auto leg_range = route->list();
    BOOST_CHECK(std::distance(leg_range.begin(), leg_range.end()) == 2);
    auto route2 =
        timetable_router(transit::date::Time("00:00:00"), transit::StopID{0}, transit::StopID{9});
    BOOST_CHECK((bool)route2);

    // when cyclic times routes are implemented, this will return a route again
    auto route3 =
        timetable_router(transit::date::Time("36:00:00"), transit::StopID{0}, transit::StopID{7});
    BOOST_CHECK((bool)!route3);
}
