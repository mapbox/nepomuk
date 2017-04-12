#include "service/earliest_arrival_parameters.hpp"

#include <cmath>
#include <utility>

namespace transit
{
namespace service
{

EarliestArrivalParameters::EarliestArrivalParameters(geometric::WGS84Coordinate _origin,
                                                     geometric::WGS84Coordinate _destination,
                                                     std::string const &,
                                                     date::Time _departure)
    : _origin(std::move(_origin)), _destination(std::move(_destination)),
      _departure(std::move(_departure))
{
}

bool EarliestArrivalParameters::valid() const
{
    // make sure the date /time format is actually reasonable (gtfs allows for times >= 24, but we
    // hide this fact from the outside world
    auto const time_is_valid = _departure < date::Time(24, 0, 0);

    // a request is valid if both coordinates are reasonable and the time format matches what we
    // expect it to be
    return time_is_valid && _origin.valid() && _destination.valid();
}

geometric::WGS84Coordinate EarliestArrivalParameters::origin() const { return _origin; }
geometric::WGS84Coordinate EarliestArrivalParameters::destination() const { return _destination; }
date::Time EarliestArrivalParameters::departure() const { return _departure; }

} // namespace service
} // namespace transit
