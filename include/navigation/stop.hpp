#ifndef NEPOMUK_NAVIGATION_STOP_HPP_
#define NEPOMUK_NAVIGATION_STOP_HPP_

#include "date/time.hpp"
#include "id/stop.hpp"

namespace nepomuk
{
namespace navigation
{

class Stop
{
  public:
    StopID id() const;
    date::UTCTimestamp departure() const;
    date::UTCTimestamp arrival() const;
    std::size_t offset() const;

  private:
    // the stop/station that has been reached
    StopID _id;
    std::size_t _offset;

    // the arrival/departure at the stop
    date::UTCTimestamp _arrival;
    date::UTCTimestamp _departure;

    friend class RoutingAlgorithm;
};
} // namespace navigation
} // namespace nepomuk

#endif // NEPOMUK_NAVIGATION_STOP_HPP_
