#include <string>

#include "tool/container/dictionary.hpp"

using namespace nepomuk::tool::container;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(basic_dictionary_functionality)
{
    Dictionary dictionary;

    BOOST_CHECK(!dictionary.contains("my"));
    BOOST_CHECK(!dictionary.contains("little"));
    BOOST_CHECK(!dictionary.contains("string"));
    BOOST_CHECK(!dictionary.contains("test"));

    dictionary.add_string("my");

    auto range = dictionary.get_lookup();
    BOOST_CHECK(std::distance(range.begin(), range.end()) == 1);
    BOOST_CHECK_EQUAL(range.begin()->first, "my");
    BOOST_CHECK_EQUAL(range.begin()->second, 0);

    dictionary.add_string("little");
    dictionary.add_string("string");
    dictionary.add_string("test");

    BOOST_CHECK(dictionary.contains("my"));
    BOOST_CHECK(dictionary.contains("little"));
    BOOST_CHECK(dictionary.contains("string"));
    BOOST_CHECK(dictionary.contains("test"));

    BOOST_CHECK(!dictionary.contains("default", "my"));
    BOOST_CHECK(!dictionary.contains("default", "little"));
    BOOST_CHECK(!dictionary.contains("default", "string"));
    BOOST_CHECK(!dictionary.contains("default", "test"));

    dictionary.add_string("default", "my");
    dictionary.add_string("default", "little");
    dictionary.add_string("default", "string");
    dictionary.add_string("default", "test");

    BOOST_CHECK(dictionary.contains("default", "my"));
    BOOST_CHECK(dictionary.contains("default", "little"));
    BOOST_CHECK(dictionary.contains("default", "string"));
    BOOST_CHECK(dictionary.contains("default", "test"));

    const auto my = dictionary.get_id("my");
    const auto little = dictionary.get_id("little");
    const auto str = dictionary.get_id("string");
    const auto test = dictionary.get_id("test");
    const auto dmy = dictionary.get_id("default", "my");
    const auto dlittle = dictionary.get_id("default", "little");
    const auto dstr = dictionary.get_id("default", "string");
    const auto dtest = dictionary.get_id("default", "test");

    BOOST_CHECK_EQUAL(my, dictionary.get_id("my"));
    BOOST_CHECK_EQUAL(little, dictionary.get_id("little"));
    BOOST_CHECK_EQUAL(str, dictionary.get_id("string"));
    BOOST_CHECK_EQUAL(test, dictionary.get_id("test"));

    BOOST_CHECK_EQUAL(dictionary.get_string(my) + " " + dictionary.get_string(little) + " " +
                          dictionary.get_string(str) + " " + dictionary.get_string(test),
                      "my little string test");
    BOOST_CHECK_EQUAL(dictionary.get_string(dmy) + " " + dictionary.get_string(dlittle) + " " +
                          dictionary.get_string(dstr) + " " + dictionary.get_string(dtest),
                      "my little string test");

    BOOST_CHECK(my != dmy);
    BOOST_CHECK(little != dlittle);
    BOOST_CHECK(str != dstr);
    BOOST_CHECK(test != dtest);
}
