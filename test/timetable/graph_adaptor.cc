#include "timetable/graph_adaptor.hpp"
#include "timetable/station_table_factory.hpp"
#include "timetable/timetable_factory.hpp"

#include "fixtures.h"

#include "gtfs/dataset.hpp"
#include "gtfs/read_csv.hpp"

#include "service/master.hpp"

using namespace nepomuk;
using namespace nepomuk::timetable;
using namespace nepomuk::gtfs;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(adapt_fixture)
{
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    auto graph =
        TimetableToGraphAdaptor::adapt(master_service.timetable(), master_service.stop_to_trip());

    // 11 stops + 3 cyclic duplicates
    BOOST_CHECK_EQUAL(graph.size(), 14);

    // might be worth to eliminate duplicates, but right now we are fine
    // self loops indicate that other lines become available
    BOOST_CHECK_EQUAL(graph.edges(graph.node(0)).size(), 2);
    BOOST_CHECK_EQUAL(graph.edges(graph.node(1)).size(), 2);
    BOOST_CHECK_EQUAL(graph.edges(graph.node(2)).size(), 1);
    BOOST_CHECK_EQUAL(graph.edges(graph.node(3)).size(), 3);
    BOOST_CHECK_EQUAL(graph.edges(graph.node(4)).size(), 2);
    BOOST_CHECK_EQUAL(graph.edges(graph.node(5)).size(), 0);
    BOOST_CHECK_EQUAL(graph.edges(graph.node(6)).size(), 1);
    BOOST_CHECK_EQUAL(graph.edges(graph.node(7)).size(), 0);
    // lower line, only reachable by transfer
    BOOST_CHECK_EQUAL(graph.edges(graph.node(8)).size(), 2);
    BOOST_CHECK_EQUAL(graph.edges(graph.node(9)).size(), 2);
    BOOST_CHECK_EQUAL(graph.edges(graph.node(10)).size(), 2);
}
