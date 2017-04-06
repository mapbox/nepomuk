#include <cstdint>
#include <functional>
#include <sstream>
#include <string>

#include "id/convenience.hpp"

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

template <typename type> void checkIDType()
{
    type id_two{2}, id_three{3}, other_id_two{2};

    BOOST_CHECK(id_two == other_id_two);
    BOOST_CHECK(id_two != id_three);
    BOOST_CHECK(id_two < id_three);
    BOOST_CHECK(id_three > id_two);
    BOOST_CHECK(id_two >= other_id_two);
    BOOST_CHECK(id_two <= other_id_two);
    BOOST_CHECK(id_two <= id_three);
    BOOST_CHECK(id_three >= id_two);

    std::ostringstream oss;
    oss << id_two;
    BOOST_CHECK_EQUAL("2", oss.str());

    std::hash<std::uint64_t> control;
    std::hash<type> hash;
    BOOST_CHECK_EQUAL(control(2), hash(id_two));
}

BOOST_AUTO_TEST_CASE(check_basic_id_functionality)
{
    checkIDType<transit::DictionaryID>();

    checkIDType<transit::AgencyID>();
    checkIDType<transit::FareID>();
    checkIDType<transit::LineID>();
    checkIDType<transit::RouteID>();
    checkIDType<transit::SequenceID>();
    checkIDType<transit::ServiceID>();
    checkIDType<transit::ShapeID>();
    checkIDType<transit::StopID>();
    checkIDType<transit::TripID>();
    checkIDType<transit::BlockID>();
    checkIDType<transit::ZoneID>();
}
