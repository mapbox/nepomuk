#include "gtfs/accessibility.hpp"

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(construct_time)
{
    BOOST_CHECK(transit::gtfs::accessibility::Wheelchair::INHERIT_OR_NONE ==
                transit::gtfs::accessibility::makeWheelchair(""));
    BOOST_CHECK(transit::gtfs::accessibility::Wheelchair::INHERIT_OR_NONE ==
                transit::gtfs::accessibility::makeWheelchair("0"));
    BOOST_CHECK(transit::gtfs::accessibility::Wheelchair::SOME_OR_ALL ==
                transit::gtfs::accessibility::makeWheelchair("1"));
    BOOST_CHECK(transit::gtfs::accessibility::Wheelchair::NONE ==
                transit::gtfs::accessibility::makeWheelchair("2"));
}
