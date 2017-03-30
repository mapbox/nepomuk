#include "fixtures.h"
#include "geometric/coordinate.hpp"
#include "id/line.hpp"
#include "id/stop.hpp"
#include "service/master.hpp"

#include <vector>

using namespace transit;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace
{
geometric::WGS84Coordinate make_coordinate(double lon, double lat)
{
    return geometric::WGS84Coordinate{
        geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
        geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat)};
}
}

BOOST_AUTO_TEST_CASE(annotate_stops)
{
    service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    auto const &geometry_annotation = data_service.geometry_annotation();

    // construct once
    BOOST_CHECK(&geometry_annotation == &data_service.geometry_annotation());

    BOOST_CHECK_EQUAL(geometry_annotation.get(StopID{0}), make_coordinate(0.005, 0.0051));
    BOOST_CHECK_EQUAL(geometry_annotation.get(StopID{1}), make_coordinate(0.005, 0.0049));
    BOOST_CHECK_EQUAL(geometry_annotation.get(StopID{2}), make_coordinate(0.0052, 0.005));
    BOOST_CHECK_EQUAL(geometry_annotation.get(StopID{3}), make_coordinate(0.0053, 0.005));
    BOOST_CHECK_EQUAL(geometry_annotation.get(StopID{4}), make_coordinate(0.0054, 0.005));
    BOOST_CHECK_EQUAL(geometry_annotation.get(StopID{5}), make_coordinate(0.0055, 0.005));
    BOOST_CHECK_EQUAL(geometry_annotation.get(StopID{6}), make_coordinate(0.0053, 0.004));
}

BOOST_AUTO_TEST_CASE(annotate_lines)
{
    service::Master data_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    auto const &geometry_annotation = data_service.geometry_annotation();

    std::vector<geometric::WGS84Coordinate> expected_coordinates;
    expected_coordinates.push_back(make_coordinate(0.005, 0.005));
    expected_coordinates.push_back(make_coordinate(0.0051, 0.005));
    expected_coordinates.push_back(make_coordinate(0.0052, 0.005));
    expected_coordinates.push_back(make_coordinate(0.00525, 0.005));
    expected_coordinates.push_back(make_coordinate(0.0053, 0.005));
    expected_coordinates.push_back(make_coordinate(0.00535, 0.005));
    expected_coordinates.push_back(make_coordinate(0.0054, 0.005));
    expected_coordinates.push_back(make_coordinate(0.00545, 0.005));
    expected_coordinates.push_back(make_coordinate(0.0055, 0.005));

    auto const range = geometry_annotation.get(LineID{0}, StopID{0}, StopID{5});
    BOOST_CHECK_EQUAL_COLLECTIONS(
        range.begin(), range.end(), expected_coordinates.begin(), expected_coordinates.end());
}
