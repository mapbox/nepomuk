#include "gtfs/dataset.hpp"
#include "gtfs/provider.hpp"
#include "gtfs/read_csv.hpp"

#include "gtfs/stop.hpp"
#include "gtfs/time.hpp"
#include "gtfs/trip.hpp"

#include "timetable/timetable.hpp"
#include "timetable/timetable_factory.hpp"

#include "adaptor/dictionary.hpp"
#include "annotation/stop_info.hpp"
#include "annotation/trip.hpp"
#include "navigation/algorithm/timetable.hpp"
#include "search/stop_to_line_factory.hpp"
#include "tool/container/string_table.hpp"

#include <cstdlib>
#include <iostream>

using namespace transit;

int main(int argc, char **argv) try
{
    // transit::gtfs::CSVDiscSource source("data/example");
    transit::gtfs::CSVDiscSource source("data/berlin-gtfs");
    auto dataset = transit::gtfs::readCSV(source);
    auto const message = transit::adaptor::Dictionary::encode(dataset.dictionary);
    transit::tool::container::StringTable dictionary;
    transit::adaptor::Dictionary::decode_into(dictionary, message);

    auto const timetable = transit::timetable::TimeTableFactory::produce(dataset);

    auto const trip_look_up =
        transit::search::StopToLineFactory::produce(dataset.stops.size(), dataset.stop_times);

    navigation::algorithm::TimeTable timetable_router(timetable, trip_look_up);

    auto trip = timetable_router(gtfs::Time("00:00:00"), gtfs::StopID{0}, gtfs::StopID{1});

    transit::annotation::StopInfoTable stop_info(dataset.stops);

    transit::annotation::Trip const annotator(stop_info, dictionary);
    std::cout << annotator(trip) << std::endl;

    return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
    std::cerr << "[error] " << e.what();
    return EXIT_FAILURE;
}
