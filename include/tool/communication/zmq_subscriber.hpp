#ifndef DIRECTIONS_TRANSIT_ZMQ_SUBSCRIBER_HPP_
#define DIRECTIONS_TRANSIT_ZMQ_SUBSCRIBER_HPP_

#include <cstdint>
#include <zmq.hpp>

namespace transit {
namespace tool {
namespace communication {

template <class BaseClass> class Subscriber {
public:
  using message_type = typename BaseClass::message_type;

  Subscriber(const std::uint32_t port);

  void subscribe();

private:
  zmq::context_t zmq_context;
  zmq::socket_t zmq_socket;

  auto self() { return static_cast<BaseClas*>(this); }
  auto self() const { return static_cast<const BaseClas*>(this); }
};

template <typename BaseClas>
Subscriber::Subscriber(const std::uint32_t port) : zmq_context(/*no of threads*/ 1), zmq_socket(zmq_context, ZMQ_SUB) {
  zmq_socket.connect("tcp://127.0.0.1:" + std::to_string(port));
  zmq_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

} // namespace communication
} // namespace tool
} // namespace transit

#endif // DIRECTIONS_TRANSIT_ZMQ_SUBSCRIBER_HPP_
