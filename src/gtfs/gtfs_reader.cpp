#include "gtfs/gtfs_reader.hpp"

#include <boost/assert.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

namespace
{
bool checkFile(const boost::filesystem::path &path)
{
    if (!boost::filesystem::exists(path))
    {
        std::cerr << "[error] missing file: " << path.string() << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}
} // namespace

namespace transit
{
namespace gtfs
{

GtfsSource::GtfsSource(boost::filesystem::path base)
    : agency(base.string() + "agency.txt"), calendar(base.string() + "calendar.txt"),
      calendar_dates(base.string() + "calendar_dates.txt"), routes(base.string() + "routes.txt"),
      shapes(base.string() + "shapes.txt"), stop_times(base.string() + "stop_times.txt"),
      stops(base.string() + "stops.txt"), transfers(base.string() + "transfers.txt"),
      trips(base.string() + "trips.txt")
{
    checkFile(agency);
    checkFile(calendar);
    checkFile(calendar_dates);
    checkFile(routes);
    checkFile(shapes);
    checkFile(stop_times);
    checkFile(stops);
    checkFile(transfers);
    checkFile(trips);
}

GtfsReader::GtfsReader() : Base(1024) {}

} // namespace gtfs
} // namespace transit
