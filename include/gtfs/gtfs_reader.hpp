#ifndef TRANSIT_GTFS_GTFS_READER_HPP_
#define TRANSIT_GTFS_GTFS_READER_HPP_

#include <boost/filesystem/path.hpp>
#include <string>

#include "tool/communication/zmq_publisher.hpp"

namespace transit
{
namespace gtfs
{

struct GtfsSource
{
    boost::filesystem::path agency;
    boost::filesystem::path calendar;
    boost::filesystem::path calendar_dates;
    boost::filesystem::path routes;
    boost::filesystem::path shapes;
    boost::filesystem::path stop_times;
    boost::filesystem::path stops;
    boost::filesystem::path transfers;
    boost::filesystem::path trips;

    GtfsSource(boost::filesystem::path base);
};

// Ingest streams from a local source on disk
class GtfsReader final : public tool::communication::ZMQPublisher<GtfsReader>
{
  public:
    using Base = tool::communication::ZMQPublisher<GtfsReader>;
    using message_type = std::string;

    GtfsReader();

  private:
};

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_GTFS_READER_HPP_
