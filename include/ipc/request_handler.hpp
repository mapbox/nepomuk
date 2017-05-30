#ifndef NEPOMUK_IPC_REQUEST_HANDLER_HPP_
#define NEPOMUK_IPC_REQUEST_HANDLER_HPP_

#include <string>
#include <zmq.hpp>

#include "service/route_parameters.hpp"
#include "service/tile_parameters.hpp"

#include "ipc/coordinate.pb.h"
#include "ipc/request.pb.h"
#include "ipc/response.pb.h"

#include "geometric/coordinate.hpp"

namespace nepomuk
{
namespace ipc
{

class RequestHandler
{
  public:
    RequestHandler(std::string const &socket_id);

    // don't get copied around
    RequestHandler(RequestHandler &&) = delete;
    RequestHandler &operator=(RequestHandler &&) = delete;

    // handle towards the IPC Service
    Response operator()(service::TileParameters parameters);

    Response operator()(service::RouteParameters parameters);

    void send_shutdown();
    Response check_alive();

  private:
    zmq::context_t context;
    zmq::socket_t socket;

    // perform a request (request does not care what is inside, it's encoded in the pbf)
    Response do_request(Request const &request);
};

} // namespace ipc
} // namespace nepomuk

#endif // NEPOMUK_IPC_REQUEST_HANDLER_HPP_
