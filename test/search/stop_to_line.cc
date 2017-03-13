// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"
#include <boost/test/unit_test.hpp>

#include "gtfs/dataset.hpp"
#include "gtfs/read_csv.hpp"

#include "search/stop_to_line_factory.hpp"
#include "timetable/timetable_factory.hpp"

BOOST_AUTO_TEST_CASE(lookup_lines_from_stops)
{
    // This needs to be replaced by a dedicated fixture (see
    // https://github.com/mapbox/directions-transit/issues/37)
    transit::gtfs::CSVDiscSource source(TRANSIT_GOOGLE_EXAMPLE_FIXTURE);
    auto dataset = transit::gtfs::readCSV(source);

    auto const trip_look_up =
        transit::search::StopToLineFactory::produce(dataset.stops.size(), dataset.stop_times);

    auto const timetable = transit::timetable::TimeTableFactory::produce(dataset);

    auto trips = trip_look_up(transit::gtfs::StopID{1});
    for (auto tid : trips)
    {
        auto route = timetable.get_trip(tid);
        auto stops = route.stops.list(transit::gtfs::StopID{1});
        BOOST_CHECK(stops.end() !=
                    std::find_if(stops.begin(), stops.end(), [tid](auto const &stop) {
                        return stop.stop_id == transit::gtfs::StopID{1};
                    }));
    }
}
