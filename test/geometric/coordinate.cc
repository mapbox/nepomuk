#include <sstream>

#include "geometric/coordinate.hpp"

using namespace transit::geometric;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(coordinate_construction)
{
    Coordinate coordinate(makeLatLonFromDouble<FixedLongitude>(1.1),
                          makeLatLonFromDouble<FixedLatitude>(1.1));

    Coordinate coordinate2(makeLatLonFromDouble<FixedLongitude>(1.1),
                           makeLatLonFromDouble<FixedLatitude>(1.1));

    Coordinate coordinate3(makeLatLonFromDouble<FixedLongitude>(-1.1),
                           makeLatLonFromDouble<FixedLatitude>(-1.1));

    BOOST_CHECK(coordinate == coordinate2);
    {
        std::ostringstream oss;
        oss << coordinate;
        BOOST_CHECK_EQUAL(oss.str(), "1.1 1.1");
    }

    {
        std::ostringstream oss;
        oss << coordinate3;
        BOOST_CHECK_EQUAL(oss.str(), "-1.1 -1.1");
    }
}

BOOST_AUTO_TEST_CASE(compute_distance)
{
    Coordinate coordinate(makeLatLonFromDouble<FixedLongitude>(0.0),
                          makeLatLonFromDouble<FixedLatitude>(0.0));
    Coordinate coordinate2(makeLatLonFromDouble<FixedLongitude>(90.0),
                           makeLatLonFromDouble<FixedLatitude>(0.0));

    auto const dist = distance(coordinate,coordinate2);
    BOOST_CHECK( 10010360 < dist && dist < 10010374);
}
