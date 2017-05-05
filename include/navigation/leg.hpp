#ifndef TRANSIT_NAVIGATION_LEG_HPP_
#define TRANSIT_NAVIGATION_LEG_HPP_

#include "date/time.hpp"
#include "navigation/segment.hpp"

#include <boost/range/iterator_range_core.hpp>
#include <cstdint>
#include <vector>

namespace transit
{
namespace navigation
{

class Leg
{
  private:
    std::vector<Segment> _segments;

    friend class RoutingAlgorithm;

  public:
    date::UTCTimestamp departure() const;
    date::UTCTimestamp arrival() const;
    std::uint32_t duration() const;

    auto segments() const { return boost::make_iterator_range(_segments.begin(), _segments.end()); }
};

} // namespace navigation
} // namespace transit

#endif // TRANSIT_NAVIGATION_LEG_HPP_
