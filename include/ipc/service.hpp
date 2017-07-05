#ifndef NEPOMUK_IPC_SERVICE_HPP_
#define NEPOMUK_IPC_SERVICE_HPP_

#include "annotation/pbf.hpp"
#include "service/master.hpp"
#include "service/route.hpp"
#include "service/tile.hpp"

#include <string>
#include <zmq.hpp>

namespace nepomuk
{

namespace service
{
class Route;
class Tile;
} // namespace service

namespace ipc
{

// The IPC::Service provides the connection between the outside world, which is looking for transit
// routing, and the engine itself via an IPC layer. It starts up the basic engine and opens unix
// sockets or other connections to supply a different service with transit directions.
// Requests towards the IPC service take the form of a PBF message that is sent to the the service
// via a dedicated port.
// 
// The master service as specified here provides a way of communicating with the full nepomuk
// engine. Based on a configuration, supplied at start-up, the engine registers the different
// plugins and opens ports for them.

class ServiceConfiguration;

class Service
{
  public:
    // starts up the engine, loads all data
    Service(ServiceConfiguration const &configuration);

    // start the service (will not return until kill message is received)
    void listen(std::string const &socket);

  private:
    // masterservice offering all nepomuk functionality
    service::Master master;
    service::Route route;
    service::Tile tile;

    annotation::PBF const &pbf_annotator;

    zmq::context_t &context;
};

} // namespace ipc
} // namespace nepomuk

#endif // NEPOMUK_IPC_SERVICE_HPP_
