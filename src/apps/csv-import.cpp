#include "gtfs/dataset.hpp"
#include "gtfs/provider.hpp"
#include "gtfs/read_csv.hpp"

#include "timetable/timetable.hpp"
#include "timetable/timetable_factory.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) try
{
    transit::gtfs::CSVDiscSource source("data/example");
    auto dataset = transit::gtfs::readCSV(source);

    auto timetable = transit::timetable::TimeTableFactory::produce(dataset);

    return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
    std::cerr << "[error] " << e.what();
    return EXIT_FAILURE;
}
