#ifndef NEPOMUK_NAVIGATION_ROUTE_HPP_
#define NEPOMUK_NAVIGATION_ROUTE_HPP_

#include "date/time.hpp"
#include "navigation/leg.hpp"

#include <boost/range/iterator_range_core.hpp>
#include <cstdint>
#include <vector>

namespace nepomuk
{
namespace navigation
{

// A route is the result of a navigation query. It describes a number of stops / transits along a
// route. The route is the result of a earliest arrival query
class Route
{
  private:
    std::vector<Leg> _legs;

    // make sure routing algorithms are allowed to construct routes
    friend class RoutingAlgorithm;

  public:
    date::UTCTimestamp departure() const;
    date::UTCTimestamp arrival() const;
    std::uint32_t duration() const;

    auto legs() const { return boost::make_iterator_range(_legs.begin(), _legs.end()); }
};

} // namespace navigation
} // namespace nepomuk

#endif // NEPOMUK_NAVIGATION_ROUTE_HPP_
