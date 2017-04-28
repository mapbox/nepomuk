#ifndef TRANSIT_NAVIGATION_CONNECTION_HPP_
#define TRANSIT_NAVIGATION_CONNECTION_HPP_

#include "date/time.hpp"
#include "id/line.hpp"

#include <cstdint>

namespace transit
{
namespace navigation
{

// forward declaration for friend access
class RoutingAlgorithm;

class Connection
{
  public:
    LineID line() const;
    date::UTCTimestamp departure() const;
    date::UTCTimestamp arrival() const;
    std::uint32_t duration() const;

  private:
    LineID _line;

    date::UTCTimestamp _departure;
    date::UTCTimestamp _arrival;

    friend class RoutingAlgorithm;
};
} // namespace navigation
} // namespace transit

#endif // TRANSIT_NAVIGATION_CONNECTION_HPP_
