#include <vector>

#include "geometric/coordinate.hpp"
#include "geometric/polyline.hpp"

using namespace transit::geometric;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(encode_decode)
{
    std::vector<WGS84Coordinate> polyline;
    polyline.push_back(
        {makeLatLonFromDouble<FixedLongitude>(0), makeLatLonFromDouble<FixedLatitude>(0)});
    polyline.push_back(
        {makeLatLonFromDouble<FixedLongitude>(0.01), makeLatLonFromDouble<FixedLatitude>(0)});
    polyline.push_back(
        {makeLatLonFromDouble<FixedLongitude>(0.01), makeLatLonFromDouble<FixedLatitude>(0.01)});
    polyline.push_back(
        {makeLatLonFromDouble<FixedLongitude>(0.93), makeLatLonFromDouble<FixedLatitude>(-0.21)});

    auto const precision = 100000;

    auto encoded_line = Polyline::encode(precision, polyline);
    BOOST_CHECK_EQUAL(encoded_line, "???o}@o}@?~}i@_urD");

    auto decoded = Polyline::decode(precision, encoded_line);
    BOOST_CHECK_EQUAL_COLLECTIONS(polyline.begin(), polyline.end(), decoded.begin(), decoded.end());

    polyline.clear();
    auto encoded_line_empty = Polyline::encode(precision, polyline);
    BOOST_CHECK_EQUAL(encoded_line_empty, "");
}
