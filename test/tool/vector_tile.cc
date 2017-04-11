#include <string>
#include <vector>
#include <cstdint>

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
VectorTileValue get_int(std::int64_t val) { return {VectorTileValueType::INT, val}; }
VectorTileValue get_uint(std::uint64_t val) { return {VectorTileValueType::UINT, val}; }
VectorTileValue get_double(double val) { return {VectorTileValueType::DOUBLE, val}; }
VectorTileValue get_float(float val) { return {VectorTileValueType::FLOAT, val}; }
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

BOOST_AUTO_TEST_CASE(compare_uints)
{
    auto const a = get_uint(1);
    auto const a2 = get_uint(1);
    auto const b = get_uint(2);

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

BOOST_AUTO_TEST_CASE(compare_float)
{
    auto const a = get_float(1.);
    auto const a2 = get_float(1.);
    auto const b = get_float(2.);

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
    auto const c = get_uint(1);
    auto const d = get_double(0.);
    auto const e = get_float(0.f);
    auto const f = get_string("hi");

    BOOST_CHECK(!(a == b));
    BOOST_CHECK(!(a == c));
    BOOST_CHECK(!(a == d));
    BOOST_CHECK(!(a == e));
    BOOST_CHECK(!(a == f));
    BOOST_CHECK(!(b == a));
    BOOST_CHECK(!(b == c));
    BOOST_CHECK(!(b == d));
    BOOST_CHECK(!(b == e));
    BOOST_CHECK(!(b == f));
    BOOST_CHECK(!(c == a));
    BOOST_CHECK(!(c == b));
    BOOST_CHECK(!(c == d));
    BOOST_CHECK(!(c == e));
    BOOST_CHECK(!(c == f));
    BOOST_CHECK(!(d == a));
    BOOST_CHECK(!(d == b));
    BOOST_CHECK(!(d == c));
    BOOST_CHECK(!(d == e));
    BOOST_CHECK(!(d == f));
    BOOST_CHECK(!(e == a));
    BOOST_CHECK(!(e == b));
    BOOST_CHECK(!(e == c));
    BOOST_CHECK(!(e == d));
    BOOST_CHECK(!(e == f));
    BOOST_CHECK(!(f == a));
    BOOST_CHECK(!(f == b));
    BOOST_CHECK(!(f == c));
    BOOST_CHECK(!(f == d));
    BOOST_CHECK(!(f == e));

    BOOST_CHECK(a < b);
    BOOST_CHECK(a < c);
    BOOST_CHECK(a < d);
    BOOST_CHECK(a < e);
    BOOST_CHECK(a < f);

    BOOST_CHECK(b < c);
    BOOST_CHECK(b < d);
    BOOST_CHECK(b < e);
    BOOST_CHECK(b < f);

    BOOST_CHECK(c < d);
    BOOST_CHECK(c < e);
    BOOST_CHECK(c < f);

    BOOST_CHECK(d < e);
    BOOST_CHECK(d < f);

    BOOST_CHECK(e < f);
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
            auto const four = get_uint(5);
            auto const five = get_float(1.1f);

            tr.write(tile_writer);
            one.write(tile_writer);
            two.write(tile_writer);
            greet.write(tile_writer);
            four.write(tile_writer);
            five.write(tile_writer);
        }
    }
    {
        protozero::pbf_reader reader(buffer);
        int messages = 0;
        bool got_string = false, got_int = false, got_double = false, got_bool = false, got_uint = false, got_float = false;
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
            case 5:
                got_uint = true;
                BOOST_CHECK_EQUAL(message.get_uint64(), 5);
                break;
            case 3:
                got_double = true;
                BOOST_CHECK_EQUAL(message.get_double(), 4.);
                break;
            case 2:
                got_float = true;
                BOOST_CHECK_EQUAL(message.get_float(), 1.1f);
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
        BOOST_CHECK(messages == 6);
        BOOST_CHECK(got_string);
        BOOST_CHECK(got_int);
        BOOST_CHECK(got_double);
        BOOST_CHECK(got_bool);
        BOOST_CHECK(got_uint);
        BOOST_CHECK(got_float);
    }
}
