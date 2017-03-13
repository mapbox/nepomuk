#ifndef TRANSIT_NAVIGATION_LEG_HPP_
#define TRANSIT_NAVIGATION_LEG_HPP_

#include <cstdint>
#include <vector>

#include <boost/range/iterator_range.hpp>

#include "timetable/stop_table.hpp"
#include "gtfs/time.hpp"
#include "gtfs/stop.hpp"

namespace transit
{
namespace navigation
{

// forward declaration for friend access
class RoutingAlgorithm;

// within a trip, a leg describes a single segment between two distinct transfers. A leg can be a
// single connection (e.g. take the 100 from A to B), or a series of possible connections.
// We can immagine taking any of a series of busses, depending on which one we manage to catch in
// case of close connections. In that case we would offer take any of 100/101/231 from A to B
class Leg
{
  public:
    struct stop_type
    {
        gtfs::StopID stop_id;
        gtfs::Time arrival;
    };
    using iterator = std::vector<stop_type>::const_iterator;

    boost::iterator_range<iterator> list() const;
    gtfs::Time departure() const;

  private:
    gtfs::Time _departure;
    // the stops along the leg
    std::vector<stop_type> stops;

    // make sure routing algorithms are allowed to construct legs
    friend class RoutingAlgorithm;
};

} // namespace navigation
} // namespace transit

#endif // TRANSIT_NAVIGATION_LEG_HPP_
