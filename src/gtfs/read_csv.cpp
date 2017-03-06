#include "gtfs/read_csv.hpp"
#include "tool/container/dictionary.hpp"
#include "tool/io/csv_decoder.hpp"
#include "tool/io/serialisation.hpp"
#include "tool/io/std_line_stream.hpp"
#include "tool/io/stream_errors.hpp"

#include <boost/assert.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

namespace
{
bool checkFile(boost::filesystem::path const &path, bool const required = false)
{
    if (!boost::filesystem::exists(path))
    {
        if (required)
            throw transit::tool::io::FileNotFoundError("Missing file: " + path.string());
        return false;
    }
    else

        return true;
}

boost::optional<boost::filesystem::path> setIfExists(boost::filesystem::path const &path)
{
    if (checkFile(path))
        return {path};
    else
        return boost::none;
}

template <class header_control>
transit::tool::io::CSVDecoder<transit::tool::io::StdLineInputStream>
makeDecoder(header_control control, transit::tool::io::StdLineInputStream &stream)
{
    auto decoder = transit::tool::io::makeCSVDecoder(stream, ",");
    if (!control(decoder.header))
        throw transit::tool::io::InvalidFileError(
            "File header does not match expected header for stream: .\n");
    return decoder;
}

template <typename destination_container, typename converter_type, typename checker_type>
void readData(boost::filesystem::path const &path,
              destination_container &destination,
              converter_type converter,
              checker_type checker)
{
    std::ifstream ifs(path.string());
    transit::tool::io::StdLineInputStream checked_stream(ifs);
    destination = transit::gtfs::decodeDataFromCSV<typename destination_container::value_type>(
        converter, makeDecoder(checker, checked_stream));
}

template <typename destination_container, typename converter_type, typename checker_type>
void readData(boost::filesystem::path const &path,
              destination_container &destination,
              converter_type converter,
              checker_type checker,
              transit::tool::container::Dictionary &dictionary)
{
    std::ifstream ifs(path.string());
    transit::tool::io::StdLineInputStream checked_stream(ifs);
    destination = transit::gtfs::decodeDataFromCSV<typename destination_container::value_type>(
        converter, makeDecoder(checker, checked_stream), dictionary);
}

} // namespace

namespace transit
{
namespace gtfs
{

CSVDiscSource::CSVDiscSource(boost::filesystem::path base)
    : agency(base.string() + "/agency.txt"), calendar(base.string() + "/calendar.txt"),
      routes(base.string() + "/routes.txt"), stops(base.string() + "/stops.txt"),
      stop_times(base.string() + "/stop_times.txt"), trips(base.string() + "/trips.txt"),
      calendar_dates(setIfExists(base.string() + "/calendar_dates.txt")),
      fare_attributes(setIfExists(base.string() + "/fare_attributes.txt")),
      fare_rules(setIfExists(base.string() + "/fare_rules.txt")),
      feed_info(setIfExists(base.string() + "/feed_info.txt")),
      frequencies(setIfExists(base.string() + "/frequencies.txt")),
      shapes(setIfExists(base.string() + "/shapes.txt")),
      transfers(setIfExists(base.string() + "/transfers.txt"))

{
    // validate the source
    constexpr const bool REQUIRED = true;
    checkFile(agency, REQUIRED);
    checkFile(calendar, REQUIRED);
    checkFile(routes, REQUIRED);
    checkFile(stops, REQUIRED);
    checkFile(stop_times, REQUIRED);
    checkFile(trips, REQUIRED);
}

Dataset readCSV(CSVDiscSource const &source)
{
    Dataset data;

    std::cout << "Reading agencies..." << std::flush;
    readData(source.agency, data.agencies, makeAgency, checkAgencyCSVHeader);
    std::cout << " done." << std::endl;
    std::cout << "Reading calendar..." << std::flush;
    readData(source.calendar, data.schedules, makeWeeklySchedule, checkWeeklyScheduleCSVHeader);
    std::cout << " done." << std::endl;
    std::cout << "Reading routes..." << std::flush;
    readData(source.routes, data.routes, makeRoute, checkRouteCSVHeader, data.dictionary);
    std::cout << " done." << std::endl;
    std::cout << "Reading stops..." << std::flush;
    readData(source.stops, data.stops, makeStop, checkStopCSVHeader, data.dictionary);
    std::cout << " done." << std::endl;
    std::cout << "Reading stop times..." << std::flush;
    readData(source.stop_times, data.stop_times, makeStopTime, checkStopTimeCSVHeader);
    std::cout << " done." << std::endl;
    std::cout << "Reading trips..." << std::flush;
    readData(source.trips, data.trips, makeTrip, checkTripCSVHeader, data.dictionary);
    std::cout << " done." << std::endl;

    // read optionals
    if (source.calendar_dates)
    {
        std::cout << "Reading calendar dates..." << std::flush;
        data.calendar_dates = {std::vector<CalendarDate>()};
        readData(*source.calendar_dates,
                 *data.calendar_dates,
                 makeCalendarDate,
                 checkCalendarDateCSVHeader);
        std::cout << " done." << std::endl;
    }

    if (source.fare_attributes)
    {
        std::cout << "Reading fare attributes..." << std::flush;
        data.fare_attributes = {std::vector<FareAttribute>()};
        readData(*source.fare_attributes,
                 *data.fare_attributes,
                 makeFareAttribute,
                 checkFareAttributeCSVHeader);
        std::cout << " done." << std::endl;
    }

    if (source.fare_rules)
    {
        std::cout << "Reading fare rules..." << std::flush;
        data.fare_rules = {std::vector<FareRule>()};
        readData(*source.fare_rules, *data.fare_rules, makeFareRule, checkFareRuleCSVHeader);
        std::cout << " done." << std::endl;
    }

    if (source.feed_info)
    {
        std::cout << "Reading feed info..." << std::flush;
        std::vector<FeedInfo> tmp;
        readData(*source.feed_info, tmp, makeFeedInfo, checkFeedInfoCSVHeader);
        BOOST_ASSERT(tmp.size() == 1);
        data.feed_info = {tmp.front()};
        std::cout << " done." << std::endl;
    }

    if (source.frequencies)
    {
        std::cout << "Reading frequencies..." << std::flush;
        data.frequencies = {std::vector<Frequency>()};
        readData(*source.frequencies, *data.frequencies, makeFrequency, checkFrequencyCSVHeader);
        std::cout << " done." << std::endl;
    }

    if (source.shapes)
    {
        std::cout << "Reading shapes..." << std::flush;
        data.shapes = {std::vector<Shape>()};
        readData(*source.shapes, *data.shapes, makeShape, checkShapeCSVHeader);
        std::cout << " done." << std::endl;
    }

    if (source.transfers)
    {
        std::cout << "Reading transfers..." << std::flush;
        data.transfers = {std::vector<Transfer>()};
        readData(*source.transfers, *data.transfers, makeTransfer, checkTransferCSVHeader);
        std::cout << " done." << std::endl;
    }
    return data;
}

} // namespace gtfs
} // namespace transit
