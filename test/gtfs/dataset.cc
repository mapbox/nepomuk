#include "gtfs/dataset.hpp"
#include "geometric/coordinate.hpp"
#include "id/stop.hpp"
#include "gtfs/stop.hpp"
#include "tool/container/dictionary.hpp"
#include <cstdint>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/optional.hpp>
#include <boost/test/unit_test.hpp>

using namespace transit;

auto coordinate(double lon, double lat)
{
    return geometric::WGS84Coordinate(
        geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
        geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat));
}

gtfs::Stop
stop(std::uint64_t id, DictionaryID name, geometric::WGS84Coordinate coordinate)
{
    return {StopID{id},
            name,
            coordinate,
            boost::none,
            boost::none,
            boost::none,
            boost::none,
            boost::none,
            boost::none,
            boost::none,
            boost::none};
}

BOOST_AUTO_TEST_CASE(combine_stops_into_stations)
{
    gtfs::Dataset dataset;

    auto const first_id = dataset.dictionary.add_string("first");
    auto const second_id = dataset.dictionary.add_string("second");
    auto const third_id = dataset.dictionary.add_string("third");
    auto const fourth_id = dataset.dictionary.add_string("fourth");

    std::uint64_t count = 0;
    // connect since close and same name
    dataset.stops.push_back(stop(count++, first_id, coordinate(0, 0)));
    dataset.stops.push_back(stop(count++, first_id, coordinate(0, 0.000001)));
    dataset.stops.push_back(stop(count++, first_id, coordinate(0.000001, 0)));
    // don't connect to first three, even though very close (but different name), don't process
    dataset.stops.push_back(stop(count++, second_id, coordinate(0, 0)));
    // not close
    dataset.stops.push_back(stop(count++, third_id, coordinate(0, 0)));
    dataset.stops.push_back(stop(count++, third_id, coordinate(10, 0)));

    // preset parents
    dataset.stops.push_back(stop(count++, fourth_id, coordinate(0, 0)));
    auto stop_with_parent = stop(count++, fourth_id, coordinate(0, 0));
    dataset.stops.back().parent_station = stop_with_parent.id;
    dataset.stops.push_back(stop_with_parent);

    dataset.connect_stops_into_stations(100);

    BOOST_CHECK(!dataset.stops[0].parent_station);
    BOOST_CHECK(dataset.stops[1].parent_station &&
                (*dataset.stops[1].parent_station == dataset.stops[0].id));
    BOOST_CHECK(dataset.stops[2].parent_station &&
                (*dataset.stops[2].parent_station == dataset.stops[0].id));
    BOOST_CHECK(!dataset.stops[3].parent_station);
    BOOST_CHECK(!dataset.stops[4].parent_station);
    BOOST_CHECK(!dataset.stops[5].parent_station);
    BOOST_CHECK(dataset.stops[6].parent_station &&
                (*dataset.stops[6].parent_station == dataset.stops[7].id));
    BOOST_CHECK(!dataset.stops[7].parent_station);
}
