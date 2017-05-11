// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"
#include <boost/test/unit_test.hpp>

#include "service/interface.hpp"
#include "service/master.hpp"
#include "service/tile.hpp"
#include "service/tile_parameters.hpp"

using namespace nepomuk;

BOOST_AUTO_TEST_CASE(render_tiles)
{
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    service::Tile tileservice(master_service);
    service::ServiceParameters parameters = {service::PluginType::TILE,
                                             service::TileParameters(16384, 16383, 15)};
    service::ServiceParameters invalid_parameters = {service::PluginType::TILE,
                                                     service::TileParameters(1 << 10, 1 << 10, 11)};
    service::ServiceParameters empty_parameters = {service::PluginType::TILE,
                                                   service::TileParameters(0, 0, 12)};

    // compute a tile
    {
        auto status = tileservice(parameters);
        BOOST_CHECK(status == service::ServiceStatus::SUCCESS);
    }
    {
        auto status = tileservice(invalid_parameters);
        BOOST_CHECK(status == service::ServiceStatus::ERROR);
    }
    {
        auto status = tileservice(empty_parameters);
        BOOST_CHECK(status == service::ServiceStatus::SUCCESS);
    }

    auto const result_as_string = [](auto const &service_parameters) {
        return static_cast<std::string>(
            boost::get<service::TileParameters>(service_parameters.parameters).result());
    };

    BOOST_CHECK(result_as_string(parameters).length() >
                result_as_string(empty_parameters).length());
}
