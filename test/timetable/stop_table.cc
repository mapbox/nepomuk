#include "timetable/stop_table.hpp"
#include "date/time.hpp"
#include "id/trip.hpp"
#include "timetable/duration_table_factory.hpp"
#include "timetable/exceptions.hpp"
#include "timetable/stop_table_factory.hpp"

using namespace transit::timetable;
using namespace transit::gtfs;
using namespace transit::date;
using namespace transit;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(check_input_validity)
{
    // cannot use empty vectors
    std::vector<StopTime> data;
    BOOST_CHECK_THROW(StopTableFactory::produce(data), InvalidInputError);
    BOOST_CHECK_THROW(DurationTableFactory::produce(data.begin(), data.end()), InvalidInputError);

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
    BOOST_CHECK_THROW(DurationTableFactory::produce(data.begin(), data.end()), InvalidInputError);

    data.pop_back();
    data.push_back(second);
    BOOST_CHECK_NO_THROW(StopTableFactory::produce(data));
    BOOST_CHECK_NO_THROW(DurationTableFactory::produce(data.begin(), data.end()));

    // check equal comparison
    auto const a = StopTableFactory::produce(data);
    auto const b = StopTableFactory::produce(data);
    auto const a_d = DurationTableFactory::produce(data.begin(), data.end());
    auto const b_d = DurationTableFactory::produce(data.begin(), data.end());
    BOOST_CHECK(a == b);
    BOOST_CHECK(a_d == b_d);

    data.clear();
    data.push_back(second);
    data.push_back(first);
    BOOST_CHECK_THROW(StopTableFactory::produce(data), InvalidInputError);
    BOOST_CHECK_THROW(DurationTableFactory::produce(data.begin(), data.end()), InvalidInputError);
}
