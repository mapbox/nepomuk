#include "date/timezone.hpp"
#include <sstream>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace nepomuk::date;

BOOST_AUTO_TEST_CASE(construct_time)
{
    auto offset = Timezone::offset("America/Los_Angeles");
}
