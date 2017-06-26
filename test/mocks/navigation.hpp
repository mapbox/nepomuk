#ifndef NEPOMUK_TEST_NAVIGATION_HPP_
#define NEPOMUK_TEST_NAVIGATION_HPP_

#include "date/time.hpp"
#include "geometric/coordinate.hpp"
#include "id/stop.hpp"
#include "id/trip.hpp"
#include "navigation/connection.hpp"
#include "navigation/leg.hpp"
#include "navigation/route.hpp"
#include "navigation/routing_algorithm.hpp"
#include "navigation/segment.hpp"
#include "navigation/stop.hpp"
#include "timetable/timetable.hpp"

using namespace nepomuk;

struct MockNavigator : public navigation::RoutingAlgorithm
{
    MockNavigator(timetable::TimeTable const &time_tables)
        : navigation::RoutingAlgorithm(time_tables)
    {
    }

    navigation::Route mock_trip() const
    {
        navigation::Route route;
        navigation::Leg leg;
        navigation::segment::Walk walk;
        navigation::segment::Transit transit;
        navigation::segment::Transfer transfer;
        navigation::Connection connection =
            make_connection(TripID{0}, date::UTCTimestamp(0), date::UTCTimestamp(0));
        navigation::Stop stop =
            make_stop(StopID{0}, TripID{0}, date::UTCTimestamp(0), date::UTCTimestamp(0));
        set_origin(transfer, StopID{0});
        set_destination(transfer, StopID{0});

        add_stop(transit, stop);
        add_stop(transit, stop);
        add_stop(transit, stop);

        add_connection(transit, connection);
        add_connection(transit, connection);
        add_connection(transit, connection);
        add_connection(transit, connection);
        add_connection(transit, connection);

        set_departure(walk, date::UTCTimestamp(0));
        set_arrival(walk, date::UTCTimestamp(0));

        add_segment(leg, make_segment(walk));
        add_segment(leg, make_segment(transit));
        add_segment(leg, make_segment(transfer));
        add_segment(leg, make_segment(transit));
        add_segment(leg, make_segment(walk));

        add_leg(route, leg);
        add_leg(route, leg);

        return route;
    }

    boost::optional<navigation::Route>
    operator()(date::Time const, std::vector<ADLeg> const &, std::vector<ADLeg> const &) const
    {
        return {mock_trip()};
    }

    boost::optional<navigation::Route>
    operator()(date::Time const, StopID const, StopID const) const
    {
        return {mock_trip()};
    }
};

#endif // NEPOMUK_TEST_NAVIGATION_HPP_
