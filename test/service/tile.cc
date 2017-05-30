// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include "fixtures.h"
#include <boost/test/unit_test.hpp>

#include "service/master.hpp"
#include "service/tile.hpp"
#include "service/tile_parameters.hpp"

using namespace nepomuk;

BOOST_AUTO_TEST_CASE(render_tiles)
{
    service::Master master_service(TRANSIT_THREE_LINES_EXAMPLE_FIXTURE);

    service::Tile tileservice(master_service);
    service::TileParameters parameters(16384, 16383, 15);
    service::TileParameters empty_parameters(0, 0, 12);

    // compute a tile
    auto result = tileservice(parameters);
    BOOST_CHECK(std::string(result).size());

    auto empty_result = tileservice(empty_parameters);

    BOOST_CHECK(((std::string)(result)).length() > ((std::string)(empty_result)).length());
}
