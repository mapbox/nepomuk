#include "adaptor/dictionary.hpp"

#include "tool/container/dictionary.hpp"
#include "tool/container/string_table.hpp"

#include "id/dictionary.hpp"                   // for DictionaryID
#include <algorithm>                           // for for_each, sort
#include <boost/assert.hpp>                    // for BOOST_ASSERT
#include <boost/range/iterator_range_core.hpp> // for iterator_range
#include <cstdint>
#include <dictionary.pb.h> // for DictionaryEntry, Dict...
#include <iosfwd>          // for string
#include <string>
#include <type_traits> // for enable_if<>::type
#include <utility>
#include <vector> // for vector, vector<>::val...

namespace nepomuk
{
namespace adaptor
{

io::container::Dictionary_proto Dictionary::encode(tool::container::Dictionary const &dictionary)
{
    io::container::Dictionary_proto message;

    auto const lookup = dictionary.get_lookup();
    using LookupType = std::pair<std::string, std::uint64_t>;
    std::vector<LookupType> data(lookup.begin(), lookup.end());

    auto const by_second = [](LookupType const &lhs, LookupType const &rhs) {
        return lhs.second < rhs.second;
    };
    std::sort(data.begin(), data.end(), by_second);

    auto const to_entry = [&dictionary](auto const &value) {
        io::container::DictionaryEntry entry;
        entry.set_id(value.second);
        auto const &display = dictionary.get_string(DictionaryID{value.second});
        entry.set_display(display);

        if (display != value.first)
            entry.set_categorized(value.first);

        return entry;
    };

    auto const add_entry = [&message, &to_entry](auto const &data) {
        auto new_entry = message.add_entries();
        *new_entry = to_entry(data);
    };

    std::for_each(data.begin(), data.end(), add_entry);

    return message;
}

void Dictionary::decode_into(tool::container::Dictionary &dictionary,
                             io::container::Dictionary_proto const &message)
{
    BOOST_ASSERT(dictionary.id_look_up.empty());
    BOOST_ASSERT(dictionary.dictionary.empty());
    dictionary.dictionary.resize(message.entries_size());
    for (int message_id = 0; message_id < message.entries_size(); ++message_id)
    {
        auto const &entry = message.entries(message_id);
        dictionary.dictionary[entry.id()] = entry.display();
        if (entry.has_categorized())
            dictionary.id_look_up.insert(std::make_pair(entry.categorized(), entry.id()));
        else
            dictionary.id_look_up.insert(std::make_pair(entry.display(), entry.id()));
    }
}
void Dictionary::decode_into(tool::container::StringTable &dictionary,
                             io::container::Dictionary_proto const &message)
{
    dictionary.resize(message.entries_size());
    for (int message_id = 0; message_id < message.entries_size(); ++message_id)
    {
        auto const &entry = message.entries(message_id);
        dictionary[entry.id()] = entry.display();
    }
}

} // namespace adaptor
} // namespace nepomuk
