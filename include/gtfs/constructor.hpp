#ifndef TRANSIT_GTFS_CONSTRUCTOR_HPP_
#define TRANSIT_GTFS_CONSTRUCTOR_HPP_

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/assert.hpp>
#include <boost/optional.hpp>

namespace transit
{
namespace gtfs
{

inline std::string forward(std::string value) { return std::move(value); }

inline std::uint64_t toInt(std::string value) { return std::stoll(value); }

inline boost::optional<std::uint64_t> asOptionalInt(std::string value)
{
    if (!value.empty())
        return {static_cast<std::uint64_t>(std::stoll(value))};
    else
        return boost::none;
}

template <typename string_constructable> string_constructable constructFromString(std::string id)
{
    return string_constructable(id);
}

template <typename IDType> IDType stringToID(std::string id)
{
    BOOST_ASSERT(id != "");
    return IDType{static_cast<std::uint64_t>(std::stoll(id))};
}

template <typename IDType> boost::optional<IDType> stringToOptionalID(std::string id)
{
    if (!id.empty())
        return {IDType{static_cast<std::uint64_t>(std::stoll(id))}};
    else
        return boost::none;
}

inline boost::optional<std::string> asOptionalString(std::string value)
{
    if (!value.empty())
        return {std::move(value)};
    else
        return boost::none;
}

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

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_CONSTRUCTOR_HPP_
