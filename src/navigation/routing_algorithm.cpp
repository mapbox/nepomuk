#include "navigation/routing_algorithm.hpp"

namespace transit
{
namespace navigation
{

void RoutingAlgorithm::add_leg(Trip &trip, Leg leg) const
{
    trip.legs.push_back(std::move(leg));
}

void RoutingAlgorithm::add_stop(Leg &leg, Leg::stop_type stop) const
{
    leg.stops.push_back(std::move(stop));
}

void RoutingAlgorithm::set_departure(Leg &leg, gtfs::Time time) const
{
    leg._departure = time;
}

} // namespace navigation
} // namespace transit
