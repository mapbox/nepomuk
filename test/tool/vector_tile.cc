#include <string>
#include <vector>

#include "geometric/coordinate.hpp"
#include "tool/container/mapbox_vector_tile.hpp"

#include <protozero/pbf_reader.hpp>
#include <protozero/pbf_writer.hpp>

using namespace transit::tool::container;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

VectorTileValue get_string(std::string const &string)
{
    return {VectorTileValueType::STRING, string};
}
VectorTileValue get_int(int val) { return {VectorTileValueType::INT, val}; }
VectorTileValue get_double(double val) { return {VectorTileValueType::DOUBLE, val}; }
VectorTileValue get_bool(bool flag) { return {VectorTileValueType::BOOL, flag}; }

BOOST_AUTO_TEST_CASE(compare_strings)
{
    auto const a = get_string("a");
    auto const a2 = get_string("a");
    auto const b = get_string("b");

    BOOST_CHECK(a == a2);
    BOOST_CHECK(a < b);
}

BOOST_AUTO_TEST_CASE(compare_ints)
{
    auto const a = get_int(1);
    auto const a2 = get_int(1);
    auto const b = get_int(2);

    BOOST_CHECK(a == a2);
    BOOST_CHECK(a < b);
}

BOOST_AUTO_TEST_CASE(compare_double)
{
    auto const a = get_double(1.);
    auto const a2 = get_double(1.);
    auto const b = get_double(2.);

    BOOST_CHECK(a == a2);
    BOOST_CHECK(a < b);
}

BOOST_AUTO_TEST_CASE(compare_bool)
{
    auto const a = get_bool(false);
    auto const a2 = get_bool(false);
    auto const b = get_bool(true);

    BOOST_CHECK(a == a2);
    BOOST_CHECK(a < b);
}

BOOST_AUTO_TEST_CASE(compare_by_type)
{
    auto const a = get_bool(true);
    auto const b = get_int(1);
    auto const c = get_double(0.);
    auto const d = get_string("hi");

    BOOST_CHECK(!(a == b));
    BOOST_CHECK(!(a == c));
    BOOST_CHECK(!(a == d));
    BOOST_CHECK(!(b == a));
    BOOST_CHECK(!(b == c));
    BOOST_CHECK(!(b == d));
    BOOST_CHECK(!(c == a));
    BOOST_CHECK(!(c == b));
    BOOST_CHECK(!(c == d));
    BOOST_CHECK(!(d == a));
    BOOST_CHECK(!(d == b));
    BOOST_CHECK(!(d == c));

    BOOST_CHECK(a < b);
    BOOST_CHECK(a < c);
    BOOST_CHECK(a < d);

    BOOST_CHECK(b < c);
    BOOST_CHECK(b < d);

    BOOST_CHECK(c < d);
}

BOOST_AUTO_TEST_CASE(write_values)
{
    std::string buffer;
    {
        protozero::pbf_writer tile_writer(buffer);
        {
            auto const tr = get_bool(false);
            auto const one = get_int(3);
            auto const two = get_double(4.);
            auto const greet = get_string("hello world!");

            tr.write(tile_writer);
            one.write(tile_writer);
            two.write(tile_writer);
            greet.write(tile_writer);
        }
    }
    {
        protozero::pbf_reader reader(buffer);
        int messages = 0;
        bool got_string = false, got_int = false, got_double = false, got_bool = false;
        while (reader.next())
        {
            auto const tag = reader.tag();

            if (tag != 4) // variant tag
                continue;

            auto message = reader.get_message();
            message.next();
            switch (message.tag())
            {
            case 7:
                got_bool = true;
                BOOST_CHECK_EQUAL(message.get_bool(), false);
                break;
            case 6:
                got_int = true;
                BOOST_CHECK_EQUAL(message.get_int64(), 3);
                break;
            case 3:
                got_double = true;
                BOOST_CHECK_EQUAL(message.get_double(), 4.);
                break;
            case 1:
                got_string = true;
                BOOST_CHECK_EQUAL(message.get_string(), "hello world!");
                break;
            default:
                message.skip();
                break;
            }
            ++messages;
        }
        BOOST_CHECK(messages == 4);
        BOOST_CHECK(got_string);
        BOOST_CHECK(got_int);
        BOOST_CHECK(got_double);
        BOOST_CHECK(got_bool);
    }
}
