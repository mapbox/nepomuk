#ifndef TRANSIT_TOOL_CONTAINER_DICTIONARY_HPP_
#define TRANSIT_TOOL_CONTAINER_DICTIONARY_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "id/dictionary.hpp"
#include <boost/range/iterator_range.hpp>

namespace transit
{

namespace adaptor
{
class Dictionary;
} // namespace adaptor

namespace tool
{
namespace container
{

class Dictionary
{
  public:
    // check if a value is within the dictionary
    bool contains(std::string const &value) const;
    bool contains(std::string const &category, std::string const &value) const;

    // request the ID of a string from with the dictionary
    DictionaryID get_id(std::string const &category, std::string const &value) const;
    DictionaryID get_id(std::string const &value) const;

    // add a new entry to the dictionary
    DictionaryID add_string(std::string const &value);
    DictionaryID add_string(std::string const &category, std::string const &value);

    // look-up in the backwards direction
    std::string const &get_string(DictionaryID const id) const;

    using lookup_table = std::unordered_map<std::string, std::uint64_t>;
    using iterator = lookup_table::const_iterator;
    using iterator_range = boost::iterator_range<iterator>;

    iterator_range get_lookup() const;

    friend class transit::adaptor::Dictionary;

  private:
    std::unordered_map<std::string, std::uint64_t> id_look_up;

    DictionaryID prepare_new_entry(std::string const &value);

    // a list of the contained strings
    std::vector<std::string> dictionary;
};

} // namespace container
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_CONTAINER_DICTIONARY_HPP_
