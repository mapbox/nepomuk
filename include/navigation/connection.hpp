#ifndef NEPOMUK_NAVIGATION_CONNECTION_HPP_
#define NEPOMUK_NAVIGATION_CONNECTION_HPP_

#include "date/time.hpp"
#include "id/trip.hpp"

namespace nepomuk
{
namespace navigation
{

class Connection
{
  public:
    TripID line() const;
    date::UTCTimestamp departure() const;
    date::UTCTimestamp arrival() const;
    std::uint32_t duration() const;

  private:
    TripID _line;

    date::UTCTimestamp _departure;
    date::UTCTimestamp _arrival;

    friend class RoutingAlgorithm;
};
} // namespace navigation
} // namespace nepomuk

#endif // NEPOMUK_NAVIGATION_CONNECTION_HPP_
