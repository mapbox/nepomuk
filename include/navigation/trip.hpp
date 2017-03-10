#ifndef TRANSIT_NAVIGATION_TRIP_HPP_
#define TRANSIT_NAVIGATION_TRIP_HPP_

#include "navigation/leg.hpp"

#include <boost/range/iterator_range.hpp>

namespace transit
{
namespace navigation
{

// forward declaration for friend access
class RoutingAlgorithm;

// A trip is the result of a navigation query. It describes a number of stops / transits along a
// route. The trip is the result of a earliest arrival query
class Trip
{
    public:
        using iterator = std::vector<Leg>::const_iterator;

        boost::iterator_range<iterator> list() const;
    private:
        std::vector<Leg> legs;

    // make sure routing algorithms are allowed to construct trips
    friend class RoutingAlgorithm;
};

} // namespace navigation
} // namespace transit

#endif // TRANSIT_NAVIGATION_TRIP_HPP_
