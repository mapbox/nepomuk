#include "gtfs/dataset.hpp"
#include "gtfs/provider.hpp"
#include "gtfs/read_csv.hpp"

#include "gtfs/stop.hpp"
#include "gtfs/time.hpp"
#include "gtfs/trip.hpp"

#include "timetable/timetable.hpp"
#include "timetable/timetable_factory.hpp"

#include <cstdlib>
#include <iostream>

using namespace transit;

int main(int argc, char **argv) try
{
    // transit::gtfs::CSVDiscSource source("data/example");
    transit::gtfs::CSVDiscSource source("data/berlin-gtfs");
    auto dataset = transit::gtfs::readCSV(source);

    auto timetable = transit::timetable::TimeTableFactory::produce(dataset);

    auto trip = timetable.get_trip(gtfs::TripID{0});

    if (trip)
    {
        gtfs::Time time("00:00:00");
        for (auto const departure : trip->departures.list(time))
        {
            auto const depart = departure.getNextDeparture(time);
            std::cout << "Departing at: " << depart << std::endl;
            for (auto const stop : trip->stops.list(gtfs::StopID{0}))
            {
                std::cout << "\tHold: " << stop.stop_id << " at " << (depart + stop.delta_t)
                          << std::endl;
            }
        }
    }

    return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
    std::cerr << "[error] " << e.what();
    return EXIT_FAILURE;
}
