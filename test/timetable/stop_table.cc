#include "timetable/stop_table.hpp"
#include "timetable/exceptions.hpp"
#include "timetable/stop_table_factory.hpp"

using namespace transit::timetable;
using namespace transit::gtfs;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(check_input_validity)
{
    // cannot use empty vectors
    std::vector<StopTime> data;
    BOOST_CHECK_THROW(StopTableFactory::produce(data), InvalidInputError);

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
    data.push_back(second_invalid_trip);
    BOOST_CHECK_THROW(StopTableFactory::produce(data), InvalidInputError);

    data.pop_back();
    data.push_back(second);
    BOOST_CHECK_NO_THROW(StopTableFactory::produce(data));

    data.clear();
    data.push_back(second);
    data.push_back(first);
    BOOST_CHECK_THROW(StopTableFactory::produce(data), InvalidInputError);
}

BOOST_AUTO_TEST_CASE(check_interpolation)
{
    // cannot use empty vectors
    std::vector<StopTime> data;

    StopTime stop_time = {TripID{42},
                      Time("00:10:00"),
                      Time("00:10:00"),
                      StopID{0},
                      SequenceID{0},
                      boost::none,
                      boost::none,
                      boost::none,
                      boost::none,
                      boost::none};

    data.push_back(stop_time);
    stop_time.sequence_id = SequenceID{1};
    stop_time.stop_id = StopID{1};
    stop_time.arrival = Time("");
    stop_time.departure = Time("");
    data.push_back(stop_time);

    stop_time.sequence_id = SequenceID{2};
    stop_time.stop_id = StopID{2};
    stop_time.arrival = Time("00:20:00");
    stop_time.departure = Time("00:20:00");
    data.push_back(stop_time);

    const auto stop_table = StopTableFactory::produce(data);
    auto range = stop_table.list(StopID{0});
    auto itr = range.begin();
    BOOST_CHECK(std::distance(range.begin(),range.end()) == 3);
    BOOST_CHECK_EQUAL(itr->stop_id,StopID{0});
    BOOST_CHECK_EQUAL(itr->delta_t,0);

    ++itr;
    BOOST_CHECK_EQUAL(itr->stop_id,StopID{1});
    BOOST_CHECK_EQUAL(itr->delta_t,300);

    ++itr;
    BOOST_CHECK_EQUAL(itr->stop_id,StopID{2});
    BOOST_CHECK_EQUAL(itr->delta_t,600);

    BOOST_CHECK_EQUAL(stop_table.trip_id(),TripID{42});
}


