#include "gtfs/dataset.hpp"
#include "gtfs/provider.hpp"
#include "gtfs/read_csv.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) try
{
    transit::gtfs::CSVDiscSource source("data/berlin-gtfs");
    auto dataset = transit::gtfs::readCSV(source);
    return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
    std::cerr << "[error] " << e.what();
    return EXIT_FAILURE;
}
