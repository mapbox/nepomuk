// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"
#include <boost/test/unit_test.hpp>

#include "gtfs/dataset.hpp"
#include "gtfs/read_csv.hpp"
#include "gtfs/time.hpp"

#include "search/stop_to_line_factory.hpp"
#include "timetable/timetable_factory.hpp"
#include "service/master.hpp"

BOOST_AUTO_TEST_CASE(lookup_lines_from_stops)
{
    // This needs to be replaced by a dedicated fixture (see
    // https://github.com/mapbox/directions-transit/issues/37)
    transit::service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    auto const timetable = master_service.timetable();
    auto const trip_look_up = master_service.stop_to_line();

    auto trips = trip_look_up(transit::StopID{1});
    for (auto tid : trips)
    {
        auto route = timetable.line(tid).get(transit::gtfs::Time("00:00:00"));
        BOOST_CHECK(route != boost::none);

        auto stops = route->stop_table.list();
        BOOST_CHECK(stops.end() != std::find_if(stops.begin(), stops.end(), [tid](auto const stop) {
                        return stop == transit::StopID{1};
                    }));

        auto stops_from_searches = route->stop_table.list(transit::StopID{1});
        BOOST_CHECK_EQUAL(*stops_from_searches.begin(), transit::StopID{1});
    }
}
