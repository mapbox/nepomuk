#ifndef NEPOMUK_GTFS_CONSTRUCTOR_HPP_
#define NEPOMUK_GTFS_CONSTRUCTOR_HPP_

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "id/dictionary.hpp"
#include "tool/container/dictionary.hpp"
#include "tool/container/id_hasher.hpp"

#include <boost/assert.hpp>
#include <boost/optional.hpp>

namespace nepomuk
{
namespace gtfs
{

inline std::string forward(std::string value) { return value; }

inline std::uint64_t toInt(std::string value) { return std::stoll(value); }

template <typename string_constructable> string_constructable constructFromString(std::string id)
{
    return string_constructable(id);
}

template <typename IDType> IDType stringToID(std::string id)
{
    BOOST_ASSERT(id != "");
    return nepomuk::tool::container::id_hash::get_id<IDType>(id, "default");
}

template <typename IDType> IDType stringToIDDirect(std::string id)
{
    BOOST_ASSERT(id != "");
    return IDType{static_cast<std::uint64_t>(std::stol(id))};
}

struct DictionaryConverter
{
    DictionaryConverter(tool::container::Dictionary &dictionary) : dictionary(dictionary) {}

    // add an entry to the dictionary, or find its ID
    DictionaryID operator()(std::string const &value)
    {
        if (!dictionary.contains(value))
            return dictionary.add_string(value);
        else
            return dictionary.get_id(value);
    }

    tool::container::Dictionary &dictionary;
};

// destroys the content of values
template <typename result_type, class Converter>
result_type construct(std::string const &key,
                      Converter converter,
                      std::map<std::string, std::size_t> const &header,
                      std::vector<std::string> &values)
{
    const auto location = header.find(key);
    if (location != header.end())
        return converter(std::move(values[location->second]));
    else
        return converter({});
}

// destroys the content of values
template <typename result_type, bool accept_empty, class Converter>
boost::optional<result_type> construct_as_optional(std::string const &key,
                                                   Converter converter,
                                                   std::map<std::string, std::size_t> const &header,
                                                   std::vector<std::string> &values)
{
    const auto location = header.find(key);
    if (location != header.end())
    {
        if (values[location->second].empty() && !accept_empty)
            return boost::none;
        else
            return {construct<result_type>(key, converter, header, values)};
    }
    else
        return boost::none;
}

} // namespace gtfs
} // namespace nepomuk

#endif // NEPOMUK_GTFS_CONSTRUCTOR_HPP_
