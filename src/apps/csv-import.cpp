#include "gtfs/dataset.hpp"
#include "gtfs/read_csv.hpp"

#include "geometric/coordinate.hpp"
#include "gtfs/time.hpp"
#include "id/stop.hpp"

#include "annotation/osrm.hpp"
#include "annotation/stop_info.hpp"
#include "annotation/trip.hpp"
#include "navigation/algorithm/timetable.hpp"
#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "service/master.hpp"
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

    {
        auto coordinate_lookup = data_service.coordinate_to_stop();

#if 1
        navigation::algorithm::TimeTableDijkstra timetable_router(data_service.timetable(),
                                                                  data_service.stop_to_line());
#else
        navigation::algorithm::TimeTable timetable_router(data_service.timetable(),
                                                          data_service.stop_to_line());
#endif
        auto stop_info = data_service.stop_info_annotation();

        auto to_coordinate = [](std::string const &line) {
            std::istringstream iss(line);
            double lat, lon;
            iss >> lat >> lon;
            return geometric::WGS84Coordinate(
                geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
                geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat));
        };

        auto annotator = data_service.trip_annotation();
        transit::annotation::OSRM const &osrm_annotator = data_service.osrm_annotation();

        while (true)
        {
            std::cout << "Enter Source..." << std::flush;
            std::string line;
            std::getline(std::cin, line);
            // auto source = StopID{static_cast<std::uint64_t>(std::stoi(line))};
            auto source = coordinate_lookup.nearest(to_coordinate(line));
            std::cout << "Enter Target..." << std::flush;
            std::getline(std::cin, line);
            // auto target = StopID{static_cast<std::uint64_t>(std::stoi(line))};
            auto target = coordinate_lookup.nearest(to_coordinate(line));
            std::cout << "Enter Departure..." << std::flush;
            std::getline(std::cin, line);
            gtfs::Time time(line);

            tool::status::Timer query_timer;
            query_timer.start();
            auto trip = timetable_router(time, source, target);
            query_timer.stop();

            tool::status::Timer annotation_timer;
            annotation_timer.start();
            if (trip)
            {
                std::cout << annotator(*trip) << std::endl;
                std::cout << osrm_annotator(*trip) << std::endl;
            }
            annotation_timer.stop();

            std::cout << "Query took: " << query_timer.milliseconds()
                      << " milliseconds, annotation: " << annotation_timer.milliseconds()
                      << " milliseconds" << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
    std::cerr << "[error] " << e.what();
    return EXIT_FAILURE;
}
