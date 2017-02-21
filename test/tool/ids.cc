#include <cstdint>
#include <functional>
#include <sstream>
#include <string>

#include "gtfs/agency.hpp"
#include "gtfs/fare.hpp"
#include "gtfs/route.hpp"
#include "gtfs/sequence.hpp"
#include "gtfs/service.hpp"
#include "gtfs/shape.hpp"
#include "gtfs/stop.hpp"
#include "gtfs/trip.hpp"
#include "gtfs/zone.hpp"

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

    std::hash<std::uint32_t> control;
    std::hash<type> hash;
    BOOST_CHECK_EQUAL(control(2), hash(id_two));
}

BOOST_AUTO_TEST_CASE(handle_unopened_streams)
{
    checkIDType<transit::gtfs::AgencyID>();
    checkIDType<transit::gtfs::FareID>();
    checkIDType<transit::gtfs::RouteID>();
    checkIDType<transit::gtfs::SequenceID>();
    checkIDType<transit::gtfs::ServiceID>();
    checkIDType<transit::gtfs::ShapeID>();
    checkIDType<transit::gtfs::StopID>();
    checkIDType<transit::gtfs::TripID>();
    checkIDType<transit::gtfs::BlockID>();
    checkIDType<transit::gtfs::ZoneID>();
}
