#include "geometric/coordinate.hpp"
#include "id/stop.hpp"

#include "navigation/algorithm/timetable.hpp"
#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "service/interface.hpp"
#include "service/master.hpp"
#include "service/tile.hpp"
#include "service/tile_parameters.hpp"
#include "tool/status/timing.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using namespace transit;

int main(int argc, char **argv) try
{
    std::string path = "data/example";

    if (argc > 1)
        path = argv[1];

    if (path.back() == '/')
        path.pop_back();

    transit::service::Master data_service(path);
    transit::service::Tile tile_service(data_service);

    {
        auto coordinate_lookup = data_service.coordinate_to_stop();

        navigation::algorithm::TimeTableDijkstra timetable_router(data_service.timetable(),
                                                                  data_service.stop_to_line());
        // navigation::algorithm::TimeTable timetable_router(data_service.timetable(),
        // data_service.stop_to_line());
        auto stop_info = data_service.stop_info_annotation();

        auto to_coordinate = [](std::string const &line) {
            std::istringstream iss(line);
            double lat, lon;
            iss >> lat >> lon;
            return geometric::WGS84Coordinate(
                geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
                geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat));
        };

        while (true)
        {
            std::cout << "Enter x y z" << std::flush;
            int x, y, z;
            std::cin >> x >> y >> z;

            transit::service::ServiceParameters parameters = {
                transit::service::PluginType::TILE, transit::service::TileParameters(x, y, z)};
            tile_service(parameters);

            std::cout << "[Tile]"
                      << (std::string)(
                             boost::get<transit::service::TileParameters>(parameters.parameters).result())
                      << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
    std::cerr << "[error] " << e.what();
    return EXIT_FAILURE;
}
