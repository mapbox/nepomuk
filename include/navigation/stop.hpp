#ifndef TRANSIT_NAVIGATION_STOP_HPP_
#define TRANSIT_NAVIGATION_STOP_HPP_

#include "date/time.hpp"
#include "id/stop.hpp"

namespace transit
{
namespace navigation
{

class Stop
{
  public:
    StopID id() const;
    date::UTCTimestamp departure() const;
    date::UTCTimestamp arrival() const;

  private:
    // the stop/station that has been reached
    StopID _id;

    // the arrival/departure at the stop
    date::UTCTimestamp _arrival;
    date::UTCTimestamp _departure;

    friend class RoutingAlgorithm;
};
} // namespace navigation
} // namespace transit

#endif // TRANSIT_NAVIGATION_STOP_HPP_
