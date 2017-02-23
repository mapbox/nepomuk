#ifndef DIRECTIONS_TRANSIT_ZMQ_PUBLISHER_HPP_
#define DIRECTIONS_TRANSIT_ZMQ_PUBLISHER_HPP_

#include <cstdint>
#include <zmq.hpp>

namespace transit
{
namespace tool
{
namespace communication
{

template <class BaseClass> class ZMQPublisher
{
  public:
    ZMQPublisher(std::uint32_t port);

    void publish();

  private:
    zmq::context_t zmq_context;
    zmq::socket_t zmq_socket;

    auto self() { return static_cast<BaseClass *>(this); }
    auto self() const { return static_cast<const BaseClass *>(this); }
};

template <typename BaseClass>
ZMQPublisher<BaseClass>::ZMQPublisher(const std::uint32_t port)
    : zmq_context(/*no of threads*/ 1), zmq_socket(zmq_context, ZMQ_PUB)
{
    zmq_socket.bind(("tcp://127.0.0.1:" + std::to_string(port)).c_str());
}

} // namespace communication
} // namespace tool
} // namespace transit

#endif // DIRECTIONS_TRANSIT_ZMQ_PUBLISHER_HPP_
