#ifndef TRANSIT_SERVICE_EARLIEST_ARRIVAL_PARAMETERS_HPP_
#define TRANSIT_SERVICE_EARLIEST_ARRIVAL_PARAMETERS_HPP_

#include <cstdint>
#include <string>

#include "date/time.hpp"
#include "geometric/coordinate.hpp"

namespace transit
{
namespace service
{

class EarliestArrival;

class EarliestArrivalParameters final
{
  public:
    EarliestArrivalParameters(geometric::WGS84Coordinate origin,
                              geometric::WGS84Coordinate destination,
                              std::string const &date,
                              date::Time departure);
    bool valid() const;

    geometric::WGS84Coordinate origin() const;
    geometric::WGS84Coordinate destination() const;
    date::Time departure() const;

    auto const &result() const { return _result; }
    auto &result() { return _result; }

  private:
    // the start/stop of the route in forms of coordinates
    geometric::WGS84Coordinate _origin;
    geometric::WGS84Coordinate _destination;
    date::Time _departure;

    // result in json format, serialised to string
    std::string _result;

    friend class EarliestArrival;
};

} // namespace service
} // namespace transit

#endif // TRANSIT_SERVICE_EARLIEST_ARRIVAL_PARAMETERS_HPP_
