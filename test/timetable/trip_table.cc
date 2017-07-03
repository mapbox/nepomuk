#include "timetable/exceptions.hpp"
#include "timetable/trip_table_factory.hpp"

#include "id/stop.hpp"
#include "id/trip.hpp"

using namespace nepomuk;
using namespace nepomuk::timetable;
using namespace nepomuk::gtfs;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(check_input_validity)
{
    // cannot use empty vectors
    TripTableFactory factory(0);

    std::vector<StopTime> data;
    BOOST_CHECK_THROW(factory.produce(data.begin(), data.end()), InvalidInputError);

    StopTime first = {TripID{0},
                      date::Time("00:10:00"),
                      date::Time("00:11:00"),
                      StopID{0},
                      SequenceID{0},
                      boost::none,
                      boost::none,
                      boost::none,
                      boost::none,
                      boost::none};

    StopTime second = {TripID{0},
                       date::Time("00:15:00"),
                       date::Time("00:16:00"),
                       StopID{1},
                       SequenceID{1},
                       boost::none,
                       boost::none,
                       boost::none,
                       boost::none,
                       boost::none};
    StopTime second_invalid_trip = {TripID{1},
                                    date::Time("00:15:00"),
                                    date::Time("00:15:00"),
                                    StopID{1},
                                    SequenceID{1},
                                    boost::none,
                                    boost::none,
                                    boost::none,
                                    boost::none,
                                    boost::none};

    data.push_back(first);
    data.push_back(second_invalid_trip);
    BOOST_CHECK_THROW(factory.produce(data.begin(), data.end()), InvalidInputError);

    data.pop_back();
    data.push_back(second);
    BOOST_CHECK_NO_THROW(factory.produce(data.begin(), data.end()));

    // check equal comparison
    TripTableFactory factory_valid(0);
    auto const trip_id = factory_valid.produce(data.begin(), data.end());
    auto const trip_table = factory_valid.extract();

    auto itr = trip_table(trip_id, 0);
    BOOST_CHECK_EQUAL(itr.departure(), date::Time("00:11:00"));
    BOOST_CHECK_EQUAL(itr.stop(), StopID{0});
    auto by_stop = trip_table(trip_id, StopID{0});
    BOOST_CHECK_EQUAL(itr.departure(), by_stop.departure());
    BOOST_CHECK_EQUAL(itr.stop(), by_stop.stop());
    auto by_stop_and_time = trip_table(trip_id, StopID{0}, date::Time("00:10:00"));
    BOOST_CHECK_EQUAL(itr.departure(), by_stop_and_time.departure());
    BOOST_CHECK_EQUAL(itr.stop(), by_stop_and_time.stop());
    auto copy = itr;
    auto post = ++itr;
    auto pre = copy++;

    BOOST_CHECK_EQUAL(itr.arrival(), copy.arrival());
    BOOST_CHECK_EQUAL(itr.departure(), copy.departure());
    BOOST_CHECK_EQUAL(itr.stop(), copy.stop());

    BOOST_CHECK_EQUAL(itr.stop(), StopID{1});
    BOOST_CHECK_EQUAL(itr.arrival(), date::Time("00:15:00"));

    BOOST_CHECK_EQUAL(pre.departure(), date::Time("00:11:00"));
    BOOST_CHECK_EQUAL(pre.stop(), StopID{0});

    BOOST_CHECK_EQUAL(post.stop(), StopID{1});
    BOOST_CHECK_EQUAL(post.arrival(), date::Time("00:15:00"));

    data.clear();
    data.push_back(second);
    data.push_back(first);
    BOOST_CHECK_THROW(factory.produce(data.begin(), data.end()), InvalidInputError);
}
