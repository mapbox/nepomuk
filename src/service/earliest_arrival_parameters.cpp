#include "service/earliest_arrival_parameters.hpp"

#include <utility>

namespace transit
{
namespace service
{

EarliestArrivalParameters::EarliestArrivalParameters(geometric::WGS84Coordinate origin,
                                                     geometric::WGS84Coordinate destination,
                                                     boost::optional<date::UTCTimestamp> departure,
                                                     boost::optional<date::UTCTimestamp> arrival,
                                                     double walking_radius,
                                                     double walking_speed,
                                                     double transfer_scale)
    : _origin(std::move(origin)), _destination(std::move(destination)),
      _departure(std::move(departure)), _arrival(arrival), _walking_radius(walking_radius),
      _walking_speed(walking_speed), _transfer_scale(transfer_scale)
{
}

bool EarliestArrivalParameters::valid() const
{
    // make sure the date /time format is actually reasonable (gtfs allows for times >= 24, but we
    // hide this fact from the outside world
    auto const time_is_valid = (_departure || _arrival) && !(_departure && _arrival);

    auto const walking_speed_valid = _walking_speed > 0;

    auto const transfer_scale_valid =
        _transfer_scale < 24 * 60 * 60; // maximum layover of a day, for some reason

    // a request is valid if both coordinates are reasonable and the time format matches what we
    // expect it to be
    return time_is_valid && walking_speed_valid && transfer_scale_valid && _origin.valid() &&
           _destination.valid();
}

geometric::WGS84Coordinate EarliestArrivalParameters::origin() const { return _origin; }
geometric::WGS84Coordinate EarliestArrivalParameters::destination() const { return _destination; }
boost::optional<date::UTCTimestamp> EarliestArrivalParameters::departure() const
{
    return _departure;
}
boost::optional<date::UTCTimestamp> EarliestArrivalParameters::arrival() const { return _arrival; }

double EarliestArrivalParameters::walking_radius() const { return _walking_radius; }
double EarliestArrivalParameters::walking_speed() const { return _walking_speed; }

double EarliestArrivalParameters::transfer_scale() const { return _transfer_scale; }

} // namespace service
} // namespace transit
