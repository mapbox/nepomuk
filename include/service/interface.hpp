#ifndef TRANSIT_SERVICE_INTERFACE_HPP_
#define TRANSIT_SERVICE_INTERFACE_HPP_

#include "service/earliest_arrival_parameters.hpp"
#include "service/tile_parameters.hpp"

#include <boost/variant.hpp>

namespace transit
{
namespace service
{

enum class ServiceStatus
{
    SUCCESS,
    ERROR
};

enum class PluginType
{
    TILE,
    EAP
};

// since every service requires it's own version of parameters, we pass down parameters as a
// variant. As a general guideline, we should try and limit the size of the parameters to be passed
// down so that we don't waste a lot of memory on the stack here.
// Every parameterset also needs to contain a reference to where we want to store the output.
struct ServiceParameters
{
    PluginType type;
    boost::variant<TileParameters, EarliestArrivalParameters> parameters;
};

class Interface
{
  public:
    virtual ServiceStatus operator()(ServiceParameters &parameters) const = 0;
    virtual ~Interface() {}
};

} // namespace service
} // namespace transit

#endif // TRANSIT_SERVICE_INTERFACE_HPP_
