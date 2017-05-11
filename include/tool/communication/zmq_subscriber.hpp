#ifndef NEPOMUK_ZMQ_SUBSCRIBER_HPP_
#define NEPOMUK_ZMQ_SUBSCRIBER_HPP_

#include <cstdint>
#include <zmq.hpp>

namespace nepomuk
{
namespace tool
{
namespace communication
{

template <class BaseClass> class ZMQSubscriber
{
  public:
    ZMQSubscriber(const std::uint32_t port);

    void subscribe();

  private:
    zmq::context_t zmq_context;
    zmq::socket_t zmq_socket;

    auto self() { return static_cast<BaseClas *>(this); }
    auto self() const { return static_cast<const BaseClas *>(this); }
};

template <typename BaseClass>
ZMQSubscriber<BaseClass>::ZMQSubscriber(const std::uint32_t port)
    : zmq_context(/*no of threads*/ 1), zmq_socket(zmq_context, ZMQ_SUB)
{
    zmq_socket.connect(("tcp://127.0.0.1:" + std::to_string(port)).c_str());
    zmq_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

} // namespace communication
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_ZMQ_SUBSCRIBER_HPP_
