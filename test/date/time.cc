#include "date/time.hpp"
#include <sstream>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace nepomuk::date;

BOOST_AUTO_TEST_CASE(construct_time)
{
    Time time;
    BOOST_CHECK(time.seconds_since_midnight == 0);
    time = Time(1, 3, 56);
    Time time_encoded("01:03:56");
    BOOST_CHECK_EQUAL(time.seconds_since_midnight, time_encoded.seconds_since_midnight);
}

BOOST_AUTO_TEST_CASE(check_normalisation)
{
    Time time(0, 62, 65);
    BOOST_CHECK_EQUAL(time.seconds_since_midnight, 62 * 60 + 65);
}

BOOST_AUTO_TEST_CASE(check_delta)
{
    Time lhs(1, 2, 3);
    Time rhs(2, 3, 4);

    BOOST_CHECK_EQUAL(rhs - lhs, 3600 + 60 + 1);
}

BOOST_AUTO_TEST_CASE(interpolated)
{
    Time interpolated("");
    BOOST_CHECK(interpolated.interpolated);
}

BOOST_AUTO_TEST_CASE(check_compare)
{
    Time a("00:00:01");
    Time b("00:01:00");
    Time c("01:00:00");

    BOOST_CHECK(!(a < a));
    BOOST_CHECK(a < b);
    BOOST_CHECK(a < c);
    BOOST_CHECK(b < c);
    BOOST_CHECK(a <= a);
    BOOST_CHECK(a <= b);
}

BOOST_AUTO_TEST_CASE(check_addition)
{
    Time a("00:00:01");
    Time b("00:01:00");

    Time zero("00:00:00");

    BOOST_CHECK(a + 59 == b);
    BOOST_CHECK(59 + a == b);
    BOOST_CHECK(a + (a - zero) == Time("00:00:02"));
}

BOOST_AUTO_TEST_CASE(serialisation)
{
    Time a("00:00:01");
    Time b("00:01:10");

    std::ostringstream oss;
    oss << a << " " << b;
    BOOST_CHECK_EQUAL(oss.str().c_str(), "0:00:01 0:01:10");
}

BOOST_AUTO_TEST_CASE(UTCCompare_less)
{
    UTCTimestamp zero;
    UTCTimestamp one(1);
    BOOST_CHECK(zero < one);
    BOOST_CHECK(!(one < zero));
    BOOST_CHECK(!(one < one));
    BOOST_CHECK(one <= UTCTimestamp::infinity());
}

BOOST_AUTO_TEST_CASE(UTCCompare_less_equal)
{
    UTCTimestamp one(1);
    UTCTimestamp also_one(1);
    UTCTimestamp two(2);

    BOOST_CHECK(one <= also_one);
    BOOST_CHECK(one <= two);
    BOOST_CHECK(!(two <= one));
    BOOST_CHECK(one <= UTCTimestamp::infinity());
}

BOOST_AUTO_TEST_CASE(UTCCompare_equal_plus)
{
    UTCTimestamp one(1);
    UTCTimestamp two(2);

    BOOST_CHECK_EQUAL(one + 1, two);
    BOOST_CHECK_EQUAL(1 + one, two);
}

BOOST_AUTO_TEST_CASE(UTCDifference)
{
    UTCTimestamp three(3);
    UTCTimestamp two(2);
    UTCTimestamp one(1);

    BOOST_CHECK_EQUAL(three - 1, two);
    BOOST_CHECK_EQUAL(three - two, 1);
}

BOOST_AUTO_TEST_CASE(UTCseconds_since_midnight)
{
    UTCTimestamp test(1493282472); // 8:41:12am
    BOOST_CHECK_EQUAL(test.seconds_since_midnight(), 8 * 60 * 60 + 41 * 60 + 12);
}

BOOST_AUTO_TEST_CASE(UTCseconds_since_midnight_local)
{
    UTCTimestamp test(1493282472);  // 8:41:12am
    UTCTimestamp test2(1493282600); // 8:41:12am
    BOOST_CHECK_EQUAL(test2 - test,
                      test2.seconds_since_midnight_local() - test.seconds_since_midnight_local());
}

BOOST_AUTO_TEST_CASE(UTCOut)
{
    std::ostringstream oss;
    UTCTimestamp test(1493282472); // 8:41am
    // they are the same, so sadly time can report both :(
    oss << test;
    auto utc = "Thu Apr 27 08:41:12 2017";
    BOOST_CHECK((oss.str() == utc));
}
