#include "timetable/departure_table.hpp"
#include "timetable/departure_table_factory.hpp"
#include "timetable/exceptions.hpp"

#include "gtfs/frequency.hpp"
#include "gtfs/stop.hpp"
#include "gtfs/time.hpp"
#include "id/sequence.hpp"
#include "id/stop.hpp"
#include "id/trip.hpp"

#include <algorithm>

#include <boost/optional.hpp>

using namespace transit::timetable;
using namespace transit;
using namespace transit::gtfs;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(check_input_validity_and_parsing)
{
    // cannot use empty vectors
    std::vector<Frequency> data;
    BOOST_CHECK_THROW(DepartureTableFactory::produce(data.begin(), data.end()), InvalidInputError);

    // hourly trips between 0:00 and 6:00
    data.push_back({TripID{0}, Time{"00:00:00"}, Time{"06:00:00"}, 3600, boost::none});
    // every 5 minutes until 10:00
    data.push_back({TripID{0}, Time{"06:00:00"}, Time{"10:00:00"}, 300, boost::none});
    // every 10 minutes between 10:00 and 16:00
    data.push_back({TripID{0}, Time{"10:00:00"}, Time{"16:00:00"}, 600, boost::none});
    // every 30 minutes from 16:00 to 24:00
    data.push_back({TripID{0}, Time{"16:00:00"}, Time{"24:00:00"}, 1800, boost::none});

    data.push_back({TripID{1}, Time{"00:00:00"}, Time{"06:00:00"}, 1800, boost::none});
    // every 10 minutes for most of the day
    data.push_back({TripID{1}, Time{"06:00:00"}, Time{"16:00:00"}, 600, boost::none});
    // every 30 minutes from 16:00 to 24:00
    data.push_back({TripID{1}, Time{"16:00:00"}, Time{"24:00:00"}, 1800, boost::none});

    BOOST_CHECK_THROW(DepartureTableFactory::produce(data.begin(), data.end()), InvalidInputError);

    const auto first_table = DepartureTableFactory::produce(data.begin(), data.begin() + 4);
    const auto second_table = DepartureTableFactory::produce(data.begin() + 4, data.end());

    {
        auto reachable_departures = first_table.list(Time("06:00:00"));
        BOOST_CHECK_EQUAL(std::distance(reachable_departures.begin(), reachable_departures.end()),
                          3ll);
    }
    {
        auto reachable_departures = first_table.list(Time("05:00:00"));
        BOOST_CHECK_EQUAL(std::distance(reachable_departures.begin(), reachable_departures.end()),
                          4ll);
    }
    {
        // missing the "5:00:00" train by a second
        auto reachable_departures = first_table.list(Time("05:00:01"));
        BOOST_CHECK_EQUAL(std::distance(reachable_departures.begin(), reachable_departures.end()),
                          3ll);
    }
}

BOOST_AUTO_TEST_CASE(test_departure)
{
    DepartureTable::Departure frequency_departure = {Time{"10:00:00"}, Time{"16:00:00"}, 600, 0ull};

    BOOST_CHECK_EQUAL(frequency_departure.get_next_departure(Time("00:01:23")), Time("10:00:00"));
    BOOST_CHECK_EQUAL(frequency_departure.get_next_departure(Time("10:00:00")), Time("10:00:00"));
    BOOST_CHECK_EQUAL(frequency_departure.get_next_departure(Time("10:00:01")), Time("10:10:00"));
    BOOST_CHECK_EQUAL(frequency_departure.get_next_departure(Time("10:10:01")), Time("10:20:00"));
    BOOST_CHECK_EQUAL(frequency_departure.get_next_departure(Time("10:10:00")), Time("10:10:00"));
}

BOOST_AUTO_TEST_CASE(construct_from_stops)
{
    std::vector<StopTime> data;
    BOOST_CHECK_THROW(DepartureTableFactory::produce(data.begin(), data.end()), InvalidInputError);

    StopTime first = {TripID{0},
                      Time("00:10:00"),
                      Time("00:10:00"),
                      StopID{0},
                      SequenceID{0},
                      boost::none,
                      boost::none,
                      boost::none,
                      boost::none,
                      boost::none};

    StopTime second = {TripID{0},
                       Time("00:15:00"),
                       Time("00:15:00"),
                       StopID{1},
                       SequenceID{1},
                       boost::none,
                       boost::none,
                       boost::none,
                       boost::none,
                       boost::none};

    StopTime second_invalid_trip = {TripID{1},
                                    Time("00:15:00"),
                                    Time("00:15:00"),
                                    StopID{1},
                                    SequenceID{1},
                                    boost::none,
                                    boost::none,
                                    boost::none,
                                    boost::none,
                                    boost::none};

    data.push_back(first);
    data.push_back(second);
    data.push_back(second_invalid_trip);
    BOOST_CHECK_THROW(DepartureTableFactory::produce(data.begin(), data.end()), InvalidInputError);

    data.pop_back();
    const auto table = DepartureTableFactory::produce(data.begin(), data.end());

    Time time("00:02:23");
    const auto departure = table.list(time);
    BOOST_CHECK(std::distance(departure.begin(), departure.end()) == 1);
    BOOST_CHECK_EQUAL(departure.begin()->get_next_departure(time), Time("00:10:00"));
}
