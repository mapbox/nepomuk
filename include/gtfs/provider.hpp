#ifndef NEPOMUK_GTFS_PROVIDER_HPP_
#define NEPOMUK_GTFS_PROVIDER_HPP_

#include "tool/communication/zmq_publisher.hpp"

namespace nepomuk
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
} // namespace nepomuk

#endif // NEPOMUK_GTFS_PROVIDER_HPP_
