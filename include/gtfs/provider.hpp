#ifndef TRANSIT_GTFS_PROVIDER_HPP_
#define TRANSIT_GTFS_PROVIDER_HPP_

#include "tool/communication/zmq_publisher.hpp"

namespace transit
{
namespace gtfs
{

class Provider final : public tool::communication::ZMQPublisher<Provider>
{
  public:
    using Base = tool::communication::ZMQPublisher<Provider>;
    using message_type = std::string;

    Provider();

  private:
};

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_PROVIDER_HPP_
