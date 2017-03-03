#include "gtfs/time.hpp"

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(construct_time)
{
    transit::gtfs::Time time;
    BOOST_CHECK(time.seconds_since_midnight == 0);
    time = transit::gtfs::Time(1, 3, 56);
    transit::gtfs::Time time_encoded("01:03:56");
    BOOST_CHECK_EQUAL(time.seconds_since_midnight, time_encoded.seconds_since_midnight);
}

BOOST_AUTO_TEST_CASE(check_normalisation)
{
    transit::gtfs::Time time(0, 62, 65);
    BOOST_CHECK_EQUAL(time.seconds_since_midnight, 62*60+65);
}

BOOST_AUTO_TEST_CASE(check_delta)
{
    transit::gtfs::Time lhs(1, 2, 3);
    transit::gtfs::Time rhs(2, 3, 4);

    BOOST_CHECK_EQUAL(rhs - lhs, 24 * 60 + 60 + 1);
}

BOOST_AUTO_TEST_CASE(interpolated)
{
    transit::gtfs::Time interpolated("");
    BOOST_CHECK(interpolated.interpolated);
}

BOOST_AUTO_TEST_CASE(check_compare)
{
    transit::gtfs::Time a("00:00:01");
    transit::gtfs::Time b("00:01:00");
    transit::gtfs::Time c("01:00:00");

    BOOST_CHECK(!(a < a));
    BOOST_CHECK(a < b);
    BOOST_CHECK(a < c);
    BOOST_CHECK(b < c);
    BOOST_CHECK(a <= a);
    BOOST_CHECK(a <= b);
}

BOOST_AUTO_TEST_CASE(check_addition)
{
    transit::gtfs::Time a("00:00:01");
    transit::gtfs::Time b("00:01:00");

    transit::gtfs::Time zero("00:00:00");

    BOOST_CHECK(a+59 == b);
    BOOST_CHECK(59+a ==  b);
    BOOST_CHECK(a + (a-zero) == transit::gtfs::Time("00:00:02"));
}