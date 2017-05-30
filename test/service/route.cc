// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"

#include "service/master.hpp"
#include "service/route.hpp"
#include "service/route_parameters.hpp"

#include "date/time.hpp"
#include "test/toolkit.hpp"

#include <string>

#include <boost/test/unit_test.hpp>

using namespace nepomuk;

BOOST_AUTO_TEST_CASE(request_route)
{
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    service::Route route(master_service);

    date::UTCTimestamp april_thirteenth_midnight(1492127999);
    date::UTCTimestamp april_thirteenth_one_am(1492045200);

    auto const stop_a1 = make_coordinate(0.005, 0.0049);
    auto const stop_a12 = make_coordinate(0.027, 0.0049);
    auto const stop_d = make_coordinate(0.0353, 0.005);
    // this coordinate tests snapping behaviour for walking distances. If walking distance
    // computation is changed, this needs to be adjusted
    auto const far_but_not_to_far = make_coordinate(0.04, 0.0049);
    auto const out_of_bounds = make_coordinate(-180.1, 90);

    service::RouteParameters out_of_bounds1(
        out_of_bounds, stop_d, april_thirteenth_midnight, boost::none, 150.0, 1.0, 1.0);
    BOOST_CHECK_EQUAL(out_of_bounds1.walking_radius(), 150.0);
    BOOST_CHECK_EQUAL(out_of_bounds1.walking_speed(), 1.0);
    BOOST_CHECK_EQUAL(out_of_bounds1.transfer_scale(), 1.0);
    BOOST_CHECK(!out_of_bounds1.arrival());
    BOOST_CHECK(!out_of_bounds1.valid());
    service::RouteParameters out_of_bounds2(
        stop_d, out_of_bounds, april_thirteenth_midnight, boost::none, 150.0, 1.0, 1.0);
    BOOST_CHECK(!out_of_bounds2.valid());
    service::RouteParameters params_invalid_time(
        stop_d, out_of_bounds, april_thirteenth_one_am, april_thirteenth_midnight, 150.0, 1.0, 1.0);
    BOOST_CHECK(!params_invalid_time.valid());
    service::RouteParameters valid_late(
        stop_a12, stop_d, april_thirteenth_midnight, boost::none, 150.0, 1.0, 1.0);
    BOOST_CHECK(valid_late.valid());
    service::RouteParameters query(
        stop_a1, stop_d, april_thirteenth_one_am, boost::none, 150.0, 1.0, 1.0);
    BOOST_CHECK(query.valid());

    service::RouteParameters increase_radius(
        far_but_not_to_far, stop_d, april_thirteenth_one_am, boost::none, 150.0, 1.0, 1.0);

    BOOST_CHECK(route(out_of_bounds1).empty());
    BOOST_CHECK(route(out_of_bounds2).empty());
    BOOST_CHECK(route(params_invalid_time).empty());

    // BOOST_CHECK(route(valid_late).empty());
    auto query_result = route(query);
    BOOST_CHECK(!query_result.empty());

    auto radius_query_result = route(increase_radius);
    BOOST_CHECK(!radius_query_result.empty());

    // compare test navigation
    BOOST_CHECK_EQUAL(API_ANNOTATED_QUERY_1_4, master_service.api_annotation()(query_result));
}

BOOST_AUTO_TEST_CASE(request_latest_departure)
{
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    service::Route route(master_service);

    date::UTCTimestamp april_thirteenth_midnight(1492127999);
    date::UTCTimestamp april_thirteenth_one_am(1492045200);

    auto const stop_a1 = make_coordinate(0.005, 0.0049);
    auto const stop_d = make_coordinate(0.0353, 0.005);

    service::RouteParameters latest_departure(
        stop_a1, stop_d, boost::none, april_thirteenth_midnight, 150.0, 1.0, 1.0);

    BOOST_CHECK(latest_departure.valid());

    BOOST_CHECK(route(latest_departure).empty());
}
