#include "tool/container/dictionary.hpp"

#include <boost/assert.hpp>
#include <utility>

namespace nepomuk
{
namespace tool
{
namespace container
{

namespace
{

const constexpr char concat_char = '~';

std::string concatenate(std::string const &category, std::string const &value)
{
    BOOST_ASSERT(category.find_first_of(concat_char) == std::string::npos);
    BOOST_ASSERT(value.find_first_of(concat_char) == std::string::npos);
    return category + concat_char + value;
}

} // namespace

bool Dictionary::contains(std::string const &value) const { return id_look_up.count(value); }

bool Dictionary::contains(std::string const &category, std::string const &value) const
{
    // forward to single string function, look-up is based on concatenated strings
    return contains(concatenate(category, value));
}

// request the ID of a string from with the dictionary
DictionaryID Dictionary::get_id(std::string const &value) const
{
    BOOST_ASSERT(contains(value));
    return DictionaryID{id_look_up.find(value)->second};
}

DictionaryID Dictionary::get_id(std::string const &category, std::string const &value) const
{
    return get_id(concatenate(category, value));
}

// add a new entry to the dictionary
DictionaryID Dictionary::add_string(std::string const &value)
{
    auto const new_id = prepare_new_entry(value);
    dictionary.push_back(value);
    return new_id;
}

DictionaryID Dictionary::add_string(std::string const &category, std::string const &value)
{
    auto const new_id = prepare_new_entry(concatenate(category, value));
    dictionary.push_back(value);
    return new_id;
}

DictionaryID Dictionary::prepare_new_entry(std::string const &value)
{
    BOOST_ASSERT(!contains(value));
    BOOST_ASSERT(dictionary.size() == id_look_up.size());
    id_look_up.insert(std::make_pair(value, dictionary.size()));
    return DictionaryID{dictionary.size()};
}

std::string const &Dictionary::get_string(DictionaryID const id) const
{
    BOOST_ASSERT(id < DictionaryID{dictionary.size()});
    return dictionary[static_cast<std::uint64_t>(id)];
}

Dictionary::iterator_range Dictionary::get_lookup() const
{
    return boost::make_iterator_range(id_look_up.begin(), id_look_up.end());
}

} // namespace container
} // namespace tool
} // namespace nepomuk
