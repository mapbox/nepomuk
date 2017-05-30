// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"
#include <boost/test/unit_test.hpp>

#include "service/master.hpp"
#include "service/tile.hpp"

#include <fstream>
#include <iterator>

using namespace nepomuk;

BOOST_AUTO_TEST_CASE(render_tiles)
{
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    auto tileservice = std::make_shared<service::Tile>(master_service);

    service::TileParameters parameters(16384, 16383, 15);
    service::TileParameters invalid_parameters(1 << 10, 1 << 10, 11);
    service::TileParameters empty_parameters(0, 0, 12);

    // compute a tile
    auto result = (*tileservice)(parameters);
    // direct
    auto empty_result = (*tileservice)(empty_parameters);
    BOOST_CHECK(((std::string)result).length() > ((std::string)(empty_result)).length());
}

BOOST_AUTO_TEST_CASE(allocate_once)
{
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);
    BOOST_CHECK(&master_service.coordinate_to_stop() == &master_service.coordinate_to_stop());
    BOOST_CHECK(&master_service.dictionary() == &master_service.dictionary());
    BOOST_CHECK(&master_service.stop_info_annotation() == &master_service.stop_info_annotation());
    BOOST_CHECK(&master_service.components() == &master_service.components());
    BOOST_CHECK(&master_service.pbf_annotation() == &master_service.pbf_annotation());
}
