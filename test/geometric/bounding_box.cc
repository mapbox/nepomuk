#include <sstream>

#include "geometric/bounding_box.hpp"
#include "geometric/coordinate.hpp"

using namespace transit::geometric;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(wgs84_bbox)
{
    WGS84Coordinate coordinate(makeLatLonFromDouble<FixedLongitude>(0),
                               makeLatLonFromDouble<FixedLatitude>(0));

    WGS84Coordinate coordinate2(makeLatLonFromDouble<FixedLongitude>(45),
                                makeLatLonFromDouble<FixedLatitude>(60));

    WGS84BoundingBox bbox(coordinate, coordinate2);

    WGS84Coordinate coordinate3(makeLatLonFromDouble<FixedLongitude>(45),
                                makeLatLonFromDouble<FixedLatitude>(59));

    WGS84Coordinate coordinate4(makeLatLonFromDouble<FixedLongitude>(46),
                                makeLatLonFromDouble<FixedLatitude>(61));

    BOOST_CHECK(bbox.contains(coordinate3));
    BOOST_CHECK(!bbox.contains(coordinate4));
    BOOST_CHECK_EQUAL(bbox.width(), 45);
    BOOST_CHECK_EQUAL(bbox.height(), 60);
}

BOOST_AUTO_TEST_CASE(mercator_bbox)
{
    WGS84Coordinate coordinate(makeLatLonFromDouble<FixedLongitude>(0),
                               makeLatLonFromDouble<FixedLatitude>(0));

    WGS84Coordinate coordinate2(makeLatLonFromDouble<FixedLongitude>(180),
                                makeLatLonFromDouble<FixedLatitude>(90));

    MercatorBoundingBox bbox(coordinate, coordinate2);

    WGS84Coordinate coordinate3(makeLatLonFromDouble<FixedLongitude>(0),
                                makeLatLonFromDouble<FixedLatitude>(0));

    WGS84Coordinate coordinate4(makeLatLonFromDouble<FixedLongitude>(-0.1),
                                makeLatLonFromDouble<FixedLatitude>(-0.1));

    BOOST_CHECK(bbox.contains(coordinate3));
    BOOST_CHECK(!bbox.contains(coordinate4));
    BOOST_CHECK_EQUAL(bbox.width(), 128);
    BOOST_CHECK_EQUAL(bbox.height(), 128);

    MercatorCoordinate ll(coordinate);
    MercatorCoordinate ur(coordinate2);
    std::swap(ll.latitude, ur.latitude);
    MercatorBoundingBox bbox2(ll, ur);

    BOOST_CHECK(bbox2.contains(coordinate3));
    BOOST_CHECK(!bbox2.contains(coordinate4));
    BOOST_CHECK_EQUAL(bbox2.width(), 128);
    BOOST_CHECK_EQUAL(bbox2.height(), 128);
}
