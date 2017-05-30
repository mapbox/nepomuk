#ifndef NEPOMUK_SERVICE_ROUTE_PARAMETERS_HPP_
#define NEPOMUK_SERVICE_ROUTE_PARAMETERS_HPP_

#include <boost/optional.hpp>

#include "date/time.hpp"
#include "geometric/coordinate.hpp"

namespace nepomuk
{
namespace service
{

class RouteParameters final
{
  public:
    RouteParameters(geometric::WGS84Coordinate origin,
                    geometric::WGS84Coordinate destination,
                    boost::optional<date::UTCTimestamp> departure,
                    boost::optional<date::UTCTimestamp> arrival,
                    double walking_radius,
                    // adjust to prefer walking
                    double walking_speed,
                    double transfer_scale);
    bool valid() const;

    // from / to
    geometric::WGS84Coordinate origin() const;
    geometric::WGS84Coordinate destination() const;

    // when
    boost::optional<date::UTCTimestamp> departure() const;
    boost::optional<date::UTCTimestamp> arrival() const;

    // may I add walking segments? How far can stations be away from the locations
    double walking_radius() const;
    double walking_speed() const;

    // how strongly to penalise transfers
    double transfer_scale() const;

  private:
    // the start/stop of the route in forms of coordinates
    geometric::WGS84Coordinate _origin;
    geometric::WGS84Coordinate _destination;
    boost::optional<date::UTCTimestamp> _departure;
    boost::optional<date::UTCTimestamp> _arrival;

    // configuration of the start/destination/transfers
    double _walking_radius;
    double _walking_speed;
    double _transfer_scale;
};

} // namespace service
} // namespace nepomuk

#endif // NEPOMUK_SERVICE_ROUTE_PARAMETERS_HPP_
