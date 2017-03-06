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

BOOST_AUTO_TEST_CASE(convert_ints)
{
    BOOST_CHECK_EQUAL(transit::gtfs::toInt("123456789"), 123456789);
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
        "", transit::gtfs::construct<std::string>("fourth", transit::gtfs::forward, header, data));
    auto const one = transit::gtfs::construct_as_optional<std::string, false>(
        "empty", transit::gtfs::forward, header, data);
    BOOST_CHECK_EQUAL(boost::none, one);
    auto const two = transit::gtfs::construct_as_optional<std::string, true>(
        "empty", transit::gtfs::forward, header, data);
    auto comp = boost::optional<std::string>("");
    BOOST_CHECK_EQUAL(comp, two);
    auto const three = transit::gtfs::construct_as_optional<std::uint64_t, false>(
        "sixth", transit::gtfs::toInt, header, data);
    BOOST_CHECK_EQUAL(boost::none, three);
    auto const four = transit::gtfs::construct_as_optional<std::uint64_t, false>(
        "seventh", transit::gtfs::toInt, header, data);
    BOOST_CHECK_EQUAL(boost::none, four);
    auto const five = transit::gtfs::construct_as_optional<std::uint64_t, false>(
        "seventh", transit::gtfs::stringToID<std::uint64_t>, header, data);
    BOOST_CHECK_EQUAL(boost::none, five);
    auto const six = transit::gtfs::construct_as_optional<std::uint64_t, false>(
        "seventh", transit::gtfs::stringToID<std::uint64_t>, header, data);
    BOOST_CHECK_EQUAL(boost::none, six);
}
