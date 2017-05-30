#ifndef NEPOMUK_ADAPTOR_IPC_RESPONSE_TO_JSON_HPP_
#define NEPOMUK_ADAPTOR_IPC_RESPONSE_TO_JSON_HPP_

#include "ipc/response.pb.h"
#include "ipc/route_response.pb.h"
#include "ipc/tile_response.pb.h"

namespace nepomuk
{
namespace adaptor
{

class IPCResponseToJSON
{
  public:
    std::string operator()(ipc::Response const &response);
    std::string operator()(ipc::RouteResponse const &response);
    std::string operator()(ipc::TileResponse const &response);
};

} // namespace adaptor
} // namespace nepomuk

#endif // NEPOMUK_ADAPTOR_IPC_RESPONSE_TO_JSON_HPP_
