#ifndef NEPOMUK_GTFS_READ_CSV_HPP_
#define NEPOMUK_GTFS_READ_CSV_HPP_

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <string>

#include "gtfs/dataset.hpp"

namespace nepomuk
{
namespace gtfs
{

struct CSVDiscSource
{
    boost::filesystem::path agency;
    boost::filesystem::path calendar;
    boost::filesystem::path routes;
    boost::filesystem::path stops;
    boost::filesystem::path stop_times;
    boost::filesystem::path trips;

    boost::optional<boost::filesystem::path> calendar_dates;
    boost::optional<boost::filesystem::path> fare_attributes;
    boost::optional<boost::filesystem::path> fare_rules;
    boost::optional<boost::filesystem::path> feed_info;
    boost::optional<boost::filesystem::path> frequencies;
    boost::optional<boost::filesystem::path> shapes;
    boost::optional<boost::filesystem::path> transfers;

    CSVDiscSource(boost::filesystem::path base);
};

// Ingest streams from a local source on disk
Dataset readCSV(CSVDiscSource const &source);

} // namespace gtfs
} // namespace nepomuk

#endif // NEPOMUK_GTFS_READ_CSV_HPP_
