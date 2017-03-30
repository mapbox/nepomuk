#include "timetable/graph_adaptor.hpp"
#include "timetable/timetable_factory.hpp"
#include "timetable/station_table_factory.hpp"

#include "fixtures.h"

#include "gtfs/dataset.hpp"
#include "gtfs/read_csv.hpp"

#include "search/stop_to_line_factory.hpp"

using namespace transit::timetable;
using namespace transit::gtfs;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(adapt_fixture)
{
    transit::gtfs::CSVDiscSource source(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    auto dataset = transit::gtfs::readCSV(source);
    auto const timetable = transit::timetable::TimeTableFactory::produce(dataset);

    auto const trip_look_up =
        transit::search::StopToLineFactory::produce(dataset.stops.size(), timetable);

    auto graph = TimetableToGraphAdaptor::adapt(timetable,trip_look_up);

    BOOST_CHECK_EQUAL(graph.size(),11);

    // might be worth to eliminate duplicates, but right now we are fine
    // self loops indicate that other lines become available
    /*
    [0]  2 1
    [1]  2 0
    [2]  2 3 3
    [3]  3 4 4 7
    [4]  8 4 5 5
    [5]  5
    [6]  3
    [7]
    [8]  9
    [9]  10
    [10
    */
    BOOST_CHECK(graph.edges(graph.node(0)).size() == 2);
    BOOST_CHECK(graph.edges(graph.node(1)).size() == 2);
    BOOST_CHECK(graph.edges(graph.node(2)).size() == 3);
    BOOST_CHECK(graph.edges(graph.node(3)).size() == 4);
    BOOST_CHECK(graph.edges(graph.node(4)).size() == 4);
    BOOST_CHECK(graph.edges(graph.node(5)).size() == 1);
    BOOST_CHECK(graph.edges(graph.node(6)).size() == 1);
    BOOST_CHECK(graph.edges(graph.node(7)).size() == 0);
    // lower line, only reachable by transfer
    BOOST_CHECK(graph.edges(graph.node(8)).size() == 1);
    BOOST_CHECK(graph.edges(graph.node(9)).size() == 1);
    BOOST_CHECK(graph.edges(graph.node(10)).size() == 0);
}
