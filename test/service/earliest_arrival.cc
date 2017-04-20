// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"

#include "service/earliest_arrival.hpp"
#include "service/earliest_arrival_parameters.hpp"
#include "service/interface.hpp"
#include "service/master.hpp"

#include "date/time.hpp"
#include "test/toolkit.hpp"

#include <string>

#include <boost/test/unit_test.hpp>

using namespace transit;

BOOST_AUTO_TEST_CASE(request_route)
{
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    service::EarliestArrival eap(master_service);

    std::string date = "20170413";
    date::Time valid_time("23:59:59");
    date::Time invalid_time("24:00:00");
    date::Time early("01:00:00");

    auto const stop_a1 = make_coordinate(0.005, 0.0049);
    auto const stop_a12 = make_coordinate(0.027, 0.0049);
    auto const stop_d = make_coordinate(0.0353, 0.005);
    auto const out_of_bounds = make_coordinate(-180.1, 90);

    service::ServiceParameters out_of_bounds1 = {
        service::PluginType::EAP,
        service::EarliestArrivalParameters(out_of_bounds, stop_d, date, valid_time)};
    BOOST_CHECK(!boost::get<service::EarliestArrivalParameters>(out_of_bounds1.parameters).valid());
    service::ServiceParameters out_of_bounds2 = {
        service::PluginType::EAP,
        service::EarliestArrivalParameters(stop_d, out_of_bounds, date, valid_time)};
    BOOST_CHECK(!boost::get<service::EarliestArrivalParameters>(out_of_bounds2.parameters).valid());
    service::ServiceParameters params_invalid_time = {
        service::PluginType::EAP,
        service::EarliestArrivalParameters(stop_d, out_of_bounds, date, invalid_time)};
    BOOST_CHECK(
        !boost::get<service::EarliestArrivalParameters>(params_invalid_time.parameters).valid());
    service::ServiceParameters valid_late = {
        service::PluginType::EAP, service::EarliestArrivalParameters(stop_a12, stop_d, date, valid_time)};
    BOOST_CHECK(boost::get<service::EarliestArrivalParameters>(valid_late.parameters).valid());
    service::ServiceParameters query = {service::PluginType::EAP,
                                        service::EarliestArrivalParameters(stop_a1, stop_d, date, early)};
    BOOST_CHECK(boost::get<service::EarliestArrivalParameters>(query.parameters).valid());

    BOOST_CHECK(service::ServiceStatus::ERROR == eap(out_of_bounds1));
    BOOST_CHECK(service::ServiceStatus::ERROR == eap(out_of_bounds2));
    BOOST_CHECK(service::ServiceStatus::ERROR == eap(params_invalid_time));

    BOOST_CHECK(service::ServiceStatus::SUCCESS == eap(valid_late));
    BOOST_CHECK(service::ServiceStatus::SUCCESS == eap(query));

    // compare test navigation
    auto const fixture = OSRM_ANNOTATED_QUERY_1_4;
    BOOST_CHECK_EQUAL(fixture,
                      boost::get<service::EarliestArrivalParameters>(query.parameters).result());
}
