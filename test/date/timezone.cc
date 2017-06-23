#include "date/timezone.hpp"
#include "date/icu.hpp"
#include <sstream>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace nepomuk::date;

BOOST_AUTO_TEST_CASE(construct_time)
{
    icu::init();
    auto offset = Timezone::offset("America/Los_Angeles");
    BOOST_CHECK(offset);
    BOOST_CHECK_EQUAL(*offset, 8*60*60);
}
