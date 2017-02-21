#include "gtfs/date.hpp"

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(construct_date)
{
    transit::gtfs::Date date;
    BOOST_CHECK(date.day == 0);
    BOOST_CHECK(date.month == 0);
    BOOST_CHECK(date.year == 0);
    date = transit::gtfs::Date(1, 3, 2017);
    transit::gtfs::Date date_encoded("20170301");
    BOOST_CHECK_EQUAL(date.day, date_encoded.day);
    BOOST_CHECK_EQUAL(date.month, date_encoded.month);
    BOOST_CHECK_EQUAL(date.year, date_encoded.year);
}
