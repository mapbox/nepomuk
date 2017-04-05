#include <string>

#include "adaptor/dictionary.hpp"
#include "id/dictionary.hpp"
#include "tool/container/dictionary.hpp"
#include "tool/container/string_table.hpp"

using namespace transit;
using namespace transit::tool::container;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(dictionary_io)
{
    Dictionary dictionary;

    dictionary.add_string("my");
    dictionary.add_string("little");
    dictionary.add_string("string");
    dictionary.add_string("test");

    auto const message = transit::adaptor::Dictionary::encode(dictionary);

    Dictionary decoded;
    transit::adaptor::Dictionary::decode_into(decoded, message);

    StringTable table;
    transit::adaptor::Dictionary::decode_into(table, message);

    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{0}), decoded.get_string(DictionaryID{0}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{1}), decoded.get_string(DictionaryID{1}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{2}), decoded.get_string(DictionaryID{2}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{3}), decoded.get_string(DictionaryID{3}));

    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{0}), table.get_string(DictionaryID{0}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{1}), table.get_string(DictionaryID{1}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{2}), table.get_string(DictionaryID{2}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{3}), table.get_string(DictionaryID{3}));

    BOOST_CHECK_EQUAL(dictionary.get_id("my"), decoded.get_id("my"));
    BOOST_CHECK_EQUAL(dictionary.get_id("little"), decoded.get_id("little"));
    BOOST_CHECK_EQUAL(dictionary.get_id("string"), decoded.get_id("string"));
    BOOST_CHECK_EQUAL(dictionary.get_id("test"), decoded.get_id("test"));
}

BOOST_AUTO_TEST_CASE(dictionary_io_categorized)
{
    Dictionary dictionary;

    dictionary.add_string("default", "my");
    dictionary.add_string("default", "little");
    dictionary.add_string("default", "string");
    dictionary.add_string("default", "test");

    auto const message = transit::adaptor::Dictionary::encode(dictionary);

    Dictionary decoded;
    transit::adaptor::Dictionary::decode_into(decoded, message);

    StringTable table;
    transit::adaptor::Dictionary::decode_into(table, message);

    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{0}), decoded.get_string(DictionaryID{0}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{1}), decoded.get_string(DictionaryID{1}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{2}), decoded.get_string(DictionaryID{2}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{3}), decoded.get_string(DictionaryID{3}));

    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{0}), table.get_string(DictionaryID{0}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{1}), table.get_string(DictionaryID{1}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{2}), table.get_string(DictionaryID{2}));
    BOOST_CHECK_EQUAL(dictionary.get_string(DictionaryID{3}), table.get_string(DictionaryID{3}));

    BOOST_CHECK_EQUAL(dictionary.get_id("default", "my"), decoded.get_id("default", "my"));
    BOOST_CHECK_EQUAL(dictionary.get_id("default", "little"), decoded.get_id("default", "little"));
    BOOST_CHECK_EQUAL(dictionary.get_id("default", "string"), decoded.get_id("default", "string"));
    BOOST_CHECK_EQUAL(dictionary.get_id("default", "test"), decoded.get_id("default", "test"));
}
