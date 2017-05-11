// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"
#include <boost/test/unit_test.hpp>

#include "date/time.hpp"

#include "search/stop_to_line_factory.hpp"
#include "service/master.hpp"
#include "timetable/timetable_factory.hpp"

using namespace nepomuk;

BOOST_AUTO_TEST_CASE(lookup_lines_from_stops)
{
    // This needs to be replaced by a dedicated fixture (see
    // https://github.com/mapbox/nepomuk/issues/37)
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    auto const timetable = master_service.timetable();
    auto const trip_look_up = master_service.stop_to_line();

    auto trips = trip_look_up(StopID{1});
    for (auto tid : trips)
    {
        auto route = timetable.line(tid).get(StopID{1}, date::Time("00:00:00"));
        BOOST_CHECK(route != boost::none);

        BOOST_CHECK(route->stop_range.size() > 0);
        BOOST_CHECK_EQUAL(route->stop_range.front(), StopID{1});
    }
}
