#include "gtfs/dataset.hpp"
#include "gtfs/read_csv.hpp"

#include "date/time.hpp"
#include "geometric/coordinate.hpp"
#include "id/stop.hpp"

#include "annotation/osrm.hpp"
#include "annotation/stop_info.hpp"
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
            iss >> lon >> lat;
            return geometric::WGS84Coordinate(
                geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
                geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat));
        };

        transit::annotation::OSRM const &osrm_annotator = data_service.osrm_annotation();

        while (true)
        {
            std::cout << "Enter Source..." << std::flush;
            std::string line;
            std::getline(std::cin, line);
            // auto source = StopID{static_cast<std::uint64_t>(std::stoi(line))};
            auto const source = to_coordinate(line);
            auto from = coordinate_lookup.all(source, 150);
            std::cout << "Enter Target..." << std::flush;
            std::getline(std::cin, line);
            // auto target = StopID{static_cast<std::uint64_t>(std::stoi(line))};
            auto const target = to_coordinate(line);
            auto to = coordinate_lookup.all(target, 150);
            std::cout << "Enter Departure..." << std::flush;
            std::getline(std::cin, line);
            date::Time time(line);

            tool::status::Timer query_timer;
            query_timer.start();
            using input_type = navigation::RoutingAlgorithm::ADLeg;
            std::vector<input_type> origin;
            std::vector<input_type> destination;

            auto const make_converter = [](auto const base_coordinate) {
                return [base_coordinate](auto const &pair) -> input_type {
                    auto const distance = geometric::distance(base_coordinate, pair.second);
                    return {pair.first,
                            static_cast<std::uint32_t>(std::llround(2 * distance)),
                            distance};
                };
            };

            std::transform(
                from.begin(), from.end(), std::back_inserter(origin), make_converter(source));
            std::transform(
                to.begin(), to.end(), std::back_inserter(destination), make_converter(target));
            auto route = timetable_router(time, origin, destination);
            query_timer.stop();

            tool::status::Timer annotation_timer;
            annotation_timer.start();
            if (route)
            {
                std::cout << osrm_annotator(*route) << std::endl;
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
