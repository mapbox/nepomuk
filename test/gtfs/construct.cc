#include "gtfs/constructor.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/test/unit_test.hpp>

using namespace nepomuk;

BOOST_AUTO_TEST_CASE(convert_ints)
{
    BOOST_CHECK_EQUAL(gtfs::toInt("123456789"), 123456789);
}

BOOST_AUTO_TEST_CASE(not_in_header)
{
    std::map<std::string, std::size_t> header;
    header["first"] = 0;
    header["second"] = 1;
    header["third"] = 2;
    header["empty"] = 3;

    std::vector<std::string> data;
    data.push_back("123");
    data.push_back("123");
    data.push_back("123");
    data.push_back("");

    BOOST_CHECK_EQUAL(
        "", gtfs::construct<std::string>("fourth", gtfs::forward, header, data));
    auto const one = gtfs::construct_as_optional<std::string, false>(
        "empty", gtfs::forward, header, data);
    BOOST_CHECK_EQUAL(boost::none, one);
    auto const two = gtfs::construct_as_optional<std::string, true>(
        "empty", gtfs::forward, header, data);
    auto comp = boost::optional<std::string>("");
    BOOST_CHECK_EQUAL(comp, two);
    auto const three = gtfs::construct_as_optional<std::uint64_t, false>(
        "sixth", gtfs::toInt, header, data);
    BOOST_CHECK_EQUAL(boost::none, three);
    auto const four = gtfs::construct_as_optional<std::uint64_t, false>(
        "seventh", gtfs::toInt, header, data);
    BOOST_CHECK_EQUAL(boost::none, four);
    auto const five = gtfs::construct_as_optional<std::uint64_t, false>(
        "seventh", gtfs::stringToID<std::uint64_t>, header, data);
    BOOST_CHECK_EQUAL(boost::none, five);
    auto const six = gtfs::construct_as_optional<std::uint64_t, false>(
        "seventh", gtfs::stringToID<std::uint64_t>, header, data);
    BOOST_CHECK_EQUAL(boost::none, six);
}
