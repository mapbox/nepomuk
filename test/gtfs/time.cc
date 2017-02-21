#include "gtfs/time.hpp"

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(construct_time)
{
    transit::gtfs::Time time;
    BOOST_CHECK(time.hour == 0);
    BOOST_CHECK(time.minute == 0);
    BOOST_CHECK(time.second == 0);
    time = transit::gtfs::Time(1, 3, 56);
    transit::gtfs::Time time_encoded("01:03:56");
    BOOST_CHECK_EQUAL(time.hour, time_encoded.hour);
    BOOST_CHECK_EQUAL(time.minute, time_encoded.minute);
    BOOST_CHECK_EQUAL(time.second, time_encoded.second);
}
