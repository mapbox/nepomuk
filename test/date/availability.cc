#include "date/availability.hpp"
#include <sstream>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace nepomuk::date;

BOOST_AUTO_TEST_CASE(construct_time)
{
    Availability avail = {Weekday::MONDAY | Weekday::TUESDAY | Weekday::WEDNESDAY |
                          Weekday::THURSDAY | Weekday::FRIDAY | Weekday::SATURDAY |
                          Weekday::SUNDAY};
    Availability weekday = {Weekday::MONDAY | Weekday::TUESDAY | Weekday::WEDNESDAY |
                            Weekday::THURSDAY | Weekday::FRIDAY};
    Availability weekend = {Weekday::SATURDAY | Weekday::SUNDAY};
    Availability non = {0};

    BOOST_CHECK(avail(Weekday::MONDAY));
    BOOST_CHECK(avail(Weekday::TUESDAY));
    BOOST_CHECK(avail(Weekday::WEDNESDAY));
    BOOST_CHECK(avail(Weekday::THURSDAY));
    BOOST_CHECK(avail(Weekday::FRIDAY));
    BOOST_CHECK(avail(Weekday::SATURDAY));
    BOOST_CHECK(avail(Weekday::SUNDAY));

    BOOST_CHECK(weekday(Weekday::MONDAY));
    BOOST_CHECK(weekday(Weekday::TUESDAY));
    BOOST_CHECK(weekday(Weekday::WEDNESDAY));
    BOOST_CHECK(weekday(Weekday::THURSDAY));
    BOOST_CHECK(weekday(Weekday::FRIDAY));
    BOOST_CHECK(!weekday(Weekday::SATURDAY));
    BOOST_CHECK(!weekday(Weekday::SUNDAY));

    BOOST_CHECK(!weekend(Weekday::MONDAY));
    BOOST_CHECK(!weekend(Weekday::TUESDAY));
    BOOST_CHECK(!weekend(Weekday::WEDNESDAY));
    BOOST_CHECK(!weekend(Weekday::THURSDAY));
    BOOST_CHECK(!weekend(Weekday::FRIDAY));
    BOOST_CHECK(weekend(Weekday::SATURDAY));
    BOOST_CHECK(weekend(Weekday::SUNDAY));

    BOOST_CHECK(!non(Weekday::MONDAY));
    BOOST_CHECK(!non(Weekday::TUESDAY));
    BOOST_CHECK(!non(Weekday::WEDNESDAY));
    BOOST_CHECK(!non(Weekday::THURSDAY));
    BOOST_CHECK(!non(Weekday::FRIDAY));
    BOOST_CHECK(!non(Weekday::SATURDAY));
    BOOST_CHECK(!non(Weekday::SUNDAY));
}
