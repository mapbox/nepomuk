#include <string>
#include <vector>

#include "annotation/stop_info.hpp"
#include "geometric/coordinate.hpp"
#include "gtfs/stop.hpp"

#include "adaptor/dictionary.hpp"
#include "tool/container/dictionary.hpp"
#include "tool/container/string_table.hpp"

using namespace transit::annotation;
using namespace transit::gtfs;
using namespace transit::tool::container;
using namespace transit::geometric;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace
{
Stop make_stop(std::uint64_t i, DictionaryID s, transit::geometric::Coordinate c)
{
    return {StopID{i},
            s,
            c,
            boost::none,
            boost::none,
            boost::none,
            boost::none,
            boost::none,
            boost::none,
            boost::none,
            boost::none};
}
};

BOOST_AUTO_TEST_CASE(annotate_stops)
{
    std::vector<Stop> stops;
    Dictionary dictionary;
    Coordinate coordinate(makeLatLonFromDouble<FixedLongitude>(1.1),
                          makeLatLonFromDouble<FixedLatitude>(1.1));
    Coordinate coordinate2(makeLatLonFromDouble<FixedLongitude>(2.1),
                           makeLatLonFromDouble<FixedLatitude>(2.1));
    Coordinate coordinate3(makeLatLonFromDouble<FixedLongitude>(3.1),
                           makeLatLonFromDouble<FixedLatitude>(3.1));
    Coordinate coordinate4(makeLatLonFromDouble<FixedLongitude>(4.1),
                           makeLatLonFromDouble<FixedLatitude>(4.1));

    stops.push_back(make_stop(0, dictionary.add_string("first"), coordinate));
    stops.push_back(make_stop(1, dictionary.add_string("second"), coordinate2));
    stops.push_back(make_stop(2, dictionary.add_string("third"), coordinate3));
    stops.push_back(make_stop(3, dictionary.add_string("fourth"), coordinate4));

    StringTable table;
    transit::adaptor::Dictionary::decode_into(table,
                                              transit::adaptor::Dictionary::encode(dictionary));

    StopInfoTable stop_info(stops);

    auto first = stop_info.get_info(StopID{0ull});
    BOOST_CHECK_EQUAL(first.location, coordinate);
    BOOST_CHECK_EQUAL(table.get_string(first.name_id), "first");

    auto second = stop_info.get_info(StopID{1ull});
    BOOST_CHECK_EQUAL(second.location, coordinate2);
    BOOST_CHECK_EQUAL(table.get_string(second.name_id), "second");

    auto third = stop_info.get_info(StopID{2ull});
    BOOST_CHECK_EQUAL(third.location, coordinate3);
    BOOST_CHECK_EQUAL(table.get_string(third.name_id), "third");

    auto fourth = stop_info.get_info(StopID{3ull});
    BOOST_CHECK_EQUAL(fourth.location, coordinate4);
    BOOST_CHECK_EQUAL(table.get_string(fourth.name_id), "fourth");
}
