// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"
#include <boost/test/unit_test.hpp>

#include "service/interface.hpp"
#include "service/master.hpp"
#include "service/tile.hpp"

#include <fstream>
#include <iterator>

using namespace nepomuk;

BOOST_AUTO_TEST_CASE(render_tiles)
{
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    auto tileservice = std::make_shared<service::Tile>(master_service);

    service::ServiceParameters parameters = {service::PluginType::TILE,
                                             service::TileParameters(16384, 16383, 15)};
    service::ServiceParameters invalid_parameters = {service::PluginType::TILE,
                                                     service::TileParameters(1 << 10, 1 << 10, 11)};
    service::ServiceParameters empty_parameters = {service::PluginType::TILE,
                                                   service::TileParameters(0, 0, 12)};

    master_service.register_plugin("tile", tileservice);
    // replace by itself
    master_service.register_plugin("tile", tileservice);
    BOOST_CHECK(master_service.get("not-registered") == nullptr);

    // compute a tile
    {
        // using master
        auto service = master_service.get("tile");
        BOOST_CHECK(service != nullptr);
        if (service)
        {
            auto status = (*service)(parameters);
            BOOST_CHECK(status == service::ServiceStatus::SUCCESS);
            {
                // direct
                auto status = (*tileservice)(empty_parameters);
                BOOST_CHECK(status == service::ServiceStatus::SUCCESS);
                auto valid_one = static_cast<std::string>(
                    boost::get<service::TileParameters>(parameters.parameters).result());
                auto empty_one = static_cast<std::string>(
                    boost::get<service::TileParameters>(empty_parameters.parameters).result());
                BOOST_CHECK(valid_one.length() > empty_one.length());
            }
        }
    }
    {
        // using master
        auto service = master_service.get("tile");
        BOOST_CHECK(service != nullptr);
        if (service)
        {
            auto status = (*service)(invalid_parameters);
            BOOST_CHECK(status == service::ServiceStatus::ERROR);
        }
    }
    master_service.deregister("tile");
    BOOST_CHECK(master_service.get("tile") == nullptr);
}

BOOST_AUTO_TEST_CASE(allocate_once)
{
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    BOOST_CHECK(&master_service.coordinate_to_stop() == &master_service.coordinate_to_stop());
    BOOST_CHECK(&master_service.dictionary() == &master_service.dictionary());
    BOOST_CHECK(&master_service.stop_info_annotation() == &master_service.stop_info_annotation());
    BOOST_CHECK(&master_service.components() == &master_service.components());
}
