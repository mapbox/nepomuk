// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"
#include <boost/test/unit_test.hpp>

#include "gtfs/dataset.hpp"
#include "gtfs/read_csv.hpp"
#include "gtfs/time.hpp"

#include "navigation/algorithm/timetable.hpp"
#include "search/stop_to_line_factory.hpp"
#include "timetable/timetable_factory.hpp"

#include <iterator>

BOOST_AUTO_TEST_CASE(lookup_lines_from_stops)
{
    // This needs to be replaced by a dedicated fixture (see
    // https://github.com/mapbox/directions-transit/issues/37)
    transit::gtfs::CSVDiscSource source(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    auto dataset = transit::gtfs::readCSV(source);

    auto const timetable = transit::timetable::TimeTableFactory::produce(dataset);
    auto const trip_look_up =
        transit::search::StopToLineFactory::produce(dataset.stops.size(), timetable);

    transit::navigation::algorithm::TimeTable timetable_router(timetable, trip_look_up);

    auto route = timetable_router(
        transit::gtfs::Time("00:00:00"), transit::gtfs::StopID{0}, transit::gtfs::StopID{6});
    BOOST_CHECK((bool)route);
    auto leg_range = route->list();
    BOOST_CHECK(std::distance(leg_range.begin(), leg_range.end()) == 2);
    auto route2 = timetable_router(
        transit::gtfs::Time("00:00:00"), transit::gtfs::StopID{0}, transit::gtfs::StopID{9});
    BOOST_CHECK((bool)route2);

    auto route3 = timetable_router(
        transit::gtfs::Time("36:00:00"), transit::gtfs::StopID{0}, transit::gtfs::StopID{6});
    BOOST_CHECK((bool)!route3);
}
