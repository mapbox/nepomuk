#include "geometric/coordinate.hpp"
#include "id/stop.hpp"

#include "navigation/algorithm/timetable.hpp"
#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "service/master.hpp"
#include "service/tile.hpp"
#include "service/tile_parameters.hpp"
#include "tool/status/timing.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using namespace nepomuk;

int main(int argc, char **argv) try
{
    std::string path = "data/example";

    if (argc > 1)
        path = argv[1];

    if (path.back() == '/')
        path.pop_back();

    nepomuk::service::Master data_service(path);
    nepomuk::service::Tile tile_service(data_service);

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

            nepomuk::service::TileParameters parameters(x, y, z);
            auto result = tile_service(parameters);
            std::cout << "[Tile]" << (std::string)(result) << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
    std::cerr << "[error] " << e.what();
    return EXIT_FAILURE;
}
