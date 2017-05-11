#include "gtfs/accessibility.hpp"

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace nepomuk;

BOOST_AUTO_TEST_CASE(construct_time)
{
    BOOST_CHECK(gtfs::accessibility::Wheelchair::INHERIT_OR_NONE ==
                gtfs::accessibility::makeWheelchair(""));
    BOOST_CHECK(gtfs::accessibility::Wheelchair::INHERIT_OR_NONE ==
                gtfs::accessibility::makeWheelchair("0"));
    BOOST_CHECK(gtfs::accessibility::Wheelchair::SOME_OR_ALL ==
                gtfs::accessibility::makeWheelchair("1"));
    BOOST_CHECK(gtfs::accessibility::Wheelchair::NONE ==
                gtfs::accessibility::makeWheelchair("2"));
}
