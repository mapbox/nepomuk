#include "gtfs/constructor.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(convert_ints)
{
    BOOST_CHECK_EQUAL(transit::gtfs::toInt("123456789"), 123456789);

    // repeat the branch a few times so coverage does not cry about it :)
    BOOST_CHECK_EQUAL(transit::gtfs::asOptionalInt(""), boost::none);
    BOOST_CHECK_EQUAL(transit::gtfs::asOptionalInt("1"), boost::optional<std::uint64_t>(1));
    BOOST_CHECK_EQUAL(transit::gtfs::asOptionalInt(""), boost::none);
    BOOST_CHECK_EQUAL(transit::gtfs::asOptionalInt("2"),  boost::optional<std::uint64_t>(2));
    BOOST_CHECK_EQUAL(transit::gtfs::asOptionalInt(""), boost::none);
    BOOST_CHECK_EQUAL(transit::gtfs::asOptionalInt("3"), boost::optional<std::uint64_t>(3));
    auto valid_optional = transit::gtfs::asOptionalInt("123");
    BOOST_CHECK(valid_optional && *valid_optional == 123);
}

BOOST_AUTO_TEST_CASE(optional_id)
{
    // repeat a few times to make sure the path is taken enough times
    BOOST_CHECK_EQUAL(transit::gtfs::stringToOptionalID<std::uint64_t>(""), boost::none);
    BOOST_CHECK_EQUAL(transit::gtfs::stringToOptionalID<std::uint64_t>(""), boost::none);
    BOOST_CHECK_EQUAL(transit::gtfs::stringToOptionalID<std::uint64_t>(""), boost::none);
    BOOST_CHECK_EQUAL(transit::gtfs::stringToOptionalID<std::uint64_t>(""), boost::none);
    auto optional_int = transit::gtfs::stringToOptionalID<std::uint64_t>("123");
    BOOST_CHECK(optional_int && (*optional_int == 0));
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
    BOOST_CHECK_EQUAL(boost::none,
                      transit::gtfs::construct<boost::optional<std::string>>(
                          "empty", transit::gtfs::asOptionalString, header, data));
    BOOST_CHECK_EQUAL(boost::none,
                      transit::gtfs::construct<boost::optional<std::uint64_t>>(
                          "sixth", transit::gtfs::asOptionalInt, header, data));
    BOOST_CHECK_EQUAL(boost::none,
                      transit::gtfs::construct<boost::optional<std::uint64_t>>(
                          "seventh", transit::gtfs::asOptionalInt, header, data));
    BOOST_CHECK_EQUAL(
        boost::none,
        transit::gtfs::construct<boost::optional<std::uint64_t>>(
            "seventh", transit::gtfs::stringToOptionalID<std::uint64_t>, header, data));
    BOOST_CHECK_EQUAL(
        boost::none,
        transit::gtfs::construct<boost::optional<std::uint64_t>>(
            "seventh", transit::gtfs::stringToOptionalID<std::uint64_t>, header, data));
}

BOOST_AUTO_TEST_CASE(optional_string)
{
    // repeat a few times so to make coverage not complain
    BOOST_CHECK_EQUAL(transit::gtfs::asOptionalString(""), boost::none);
    BOOST_CHECK_EQUAL(transit::gtfs::asOptionalString(""), boost::none);
    BOOST_CHECK_EQUAL(transit::gtfs::asOptionalString(""), boost::none);
    BOOST_CHECK_EQUAL(transit::gtfs::asOptionalString(""), boost::none);
}
