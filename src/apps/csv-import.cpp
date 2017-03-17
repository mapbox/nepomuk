#include "gtfs/dataset.hpp"
#include "gtfs/provider.hpp"
#include "gtfs/read_csv.hpp"

#include "geometric/coordinate.hpp"
#include "gtfs/stop.hpp"
#include "gtfs/time.hpp"
#include "gtfs/trip.hpp"

#include "timetable/timetable.hpp"
#include "timetable/timetable_factory.hpp"

#include "adaptor/dictionary.hpp"
#include "annotation/stop_info.hpp"
#include "annotation/trip.hpp"
#include "navigation/algorithm/timetable.hpp"
#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "search/coordinate_to_stop.hpp"
#include "search/stop_to_line_factory.hpp"
#include "tool/container/string_table.hpp"
#include "tool/timing.hpp"

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

    transit::gtfs::CSVDiscSource source(path.c_str());

    auto dataset = transit::gtfs::readCSV(source);
    auto const message = transit::adaptor::Dictionary::encode(dataset.dictionary);
    transit::tool::container::StringTable dictionary;
    transit::adaptor::Dictionary::decode_into(dictionary, message);

    auto const timetable = transit::timetable::TimeTableFactory::produce(dataset);

    auto const trip_look_up =
        transit::search::StopToLineFactory::produce(dataset.stops.size(), timetable);

    auto make_coordinate_lookup = [&]() {
        std::vector<std::pair<geometric::Coordinate, gtfs::StopID>> data;
        std::for_each(dataset.stops.begin(), dataset.stops.end(), [&](auto const &element) {
            if (!element.location_type || *element.location_type == gtfs::LocationType::STOP)
                data.push_back(std::make_pair(element.location, element.id));
        });
        return search::CoordinateToStop(data);
    };

    auto coordinate_lookup = make_coordinate_lookup();

    navigation::algorithm::TimeTableDijkstra timetable_router(timetable, trip_look_up);
    //navigation::algorithm::TimeTable timetable_router(timetable, trip_look_up);
    transit::annotation::StopInfoTable stop_info(dataset.stops);

    auto to_coordinate = [](std::string const &line) {
        std::istringstream iss(line);
        double lat, lon;
        iss >> lat >> lon;
        return geometric::Coordinate(
            geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
            geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat));
    };

    while (true)
    {
        std::cout << "Enter Source..." << std::flush;
        std::string line;
        std::getline(std::cin, line);
        // auto source = gtfs::StopID{static_cast<std::uint64_t>(std::stoi(line))};
        auto source = coordinate_lookup.nearest(to_coordinate(line));
        std::cout << "Enter Target..." << std::flush;
        std::getline(std::cin, line);
        // auto target = gtfs::StopID{static_cast<std::uint64_t>(std::stoi(line))};
        auto target = coordinate_lookup.nearest(to_coordinate(line));
        std::cout << "Enter Departure..." << std::flush;
        std::getline(std::cin, line);
        gtfs::Time time(line);

        TIMER_START(query);
        auto trip = timetable_router(time, source, target);
        TIMER_STOP(query);
        /*
            for (std::uint64_t i = 0; i < 20; ++i)
            {
                TIMER_START(query);
                auto trip = timetable_router(gtfs::Time("00:00:00"), gtfs::StopID{i},
           gtfs::StopID{101});
                TIMER_STOP(query);
        */
        transit::annotation::Trip const annotator(stop_info, dictionary);

        TIMER_START(annotate);
        if (trip)
            std::cout << annotator(*trip) << std::endl;
        TIMER_STOP(annotate);

        std::cout << "Query took: " << TIMER_SEC(query)
                  << " seconds, annotation: " << TIMER_SEC(annotate) << std::endl;
    }

    return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
    std::cerr << "[error] " << e.what();
    return EXIT_FAILURE;
}
