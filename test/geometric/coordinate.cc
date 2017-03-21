#include <sstream>

#include "geometric/coordinate.hpp"

using namespace transit::geometric;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(coordinate_construction)
{
    WGS84Coordinate coordinate(makeLatLonFromDouble<FixedLongitude>(1.1),
                               makeLatLonFromDouble<FixedLatitude>(1.1));

    WGS84Coordinate coordinate2(makeLatLonFromDouble<FixedLongitude>(1.1),
                                makeLatLonFromDouble<FixedLatitude>(1.1));

    WGS84Coordinate coordinate3(makeLatLonFromDouble<FixedLongitude>(-1.1),
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

BOOST_AUTO_TEST_CASE(check_validity)
{
    WGS84Coordinate coordinate(makeLatLonFromDouble<FixedLongitude>(1.1),
                               makeLatLonFromDouble<FixedLatitude>(1.1));
    BOOST_CHECK(coordinate.valid());
    WGS84Coordinate coordinate2(makeLatLonFromDouble<FixedLongitude>(-180.001),
                                makeLatLonFromDouble<FixedLatitude>(0.0));
    BOOST_CHECK(!coordinate2.valid());
    WGS84Coordinate coordinate3(makeLatLonFromDouble<FixedLongitude>(180.001),
                                makeLatLonFromDouble<FixedLatitude>(0.0));
    BOOST_CHECK(!coordinate3.valid());
    WGS84Coordinate coordinate4(makeLatLonFromDouble<FixedLongitude>(180.0),
                                makeLatLonFromDouble<FixedLatitude>(0.0));
    BOOST_CHECK(coordinate4.valid());
    WGS84Coordinate coordinate5(makeLatLonFromDouble<FixedLongitude>(179.9999),
                                makeLatLonFromDouble<FixedLatitude>(90.01));
    BOOST_CHECK(!coordinate5.valid());
    WGS84Coordinate coordinate6(makeLatLonFromDouble<FixedLongitude>(179.9999),
                                makeLatLonFromDouble<FixedLatitude>(-90.01));
    BOOST_CHECK(!coordinate6.valid());
}

BOOST_AUTO_TEST_CASE(compute_distance)
{
    WGS84Coordinate coordinate(makeLatLonFromDouble<FixedLongitude>(0.0),
                               makeLatLonFromDouble<FixedLatitude>(0.0));
    WGS84Coordinate coordinate2(makeLatLonFromDouble<FixedLongitude>(90.0),
                                makeLatLonFromDouble<FixedLatitude>(0.0));

    auto const dist = distance(coordinate, coordinate2);
    BOOST_CHECK(10010360 < dist && dist < 10010374);
}

BOOST_AUTO_TEST_CASE(compare_print_mercator)
{
    std::ostringstream oss;
    WGS84Coordinate coordinate(makeLatLonFromDouble<FixedLongitude>(135),
                               makeLatLonFromDouble<FixedLatitude>(45));
    MercatorCoordinate mercator(coordinate);
    MercatorCoordinate mercator_copy(coordinate);

    oss << mercator;

    BOOST_CHECK_EQUAL(mercator, mercator_copy);
    BOOST_CHECK_EQUAL(oss.str(), "224 92.0896");
}

BOOST_AUTO_TEST_CASE(mercator_converion)
{
    WGS84Coordinate upper_left(makeLatLonFromDouble<FixedLongitude>(-180),
                               makeLatLonFromDouble<FixedLatitude>(90));

    MercatorCoordinate upper_left_merc(upper_left);
    BOOST_CHECK_EQUAL(doubleFromLatLon(upper_left_merc.longitude), 0.);
    BOOST_CHECK_EQUAL(doubleFromLatLon(upper_left_merc.latitude), 0.);
    WGS84Coordinate lower_right(makeLatLonFromDouble<FixedLongitude>(180),
                                makeLatLonFromDouble<FixedLatitude>(-90));
    MercatorCoordinate lower_right_merc(lower_right);
    BOOST_CHECK_CLOSE(doubleFromLatLon(lower_right_merc.longitude), 256, 1);
    BOOST_CHECK_CLOSE(doubleFromLatLon(lower_right_merc.latitude), 256, 1);
}
