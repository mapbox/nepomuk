#ifndef TRANSIT_TOOL_CONTAINER_ID_HASHER_HPP_
#define TRANSIT_TOOL_CONTAINER_ID_HASHER_HPP_

#include <cstdint>
#include <string>
#include <unordered_map>

namespace transit
{
namespace tool
{
namespace container
{

// the ID hahser generates a new ID for any potential type we can have and that is provided in
// string form. Since IDs have to be unique over all entries, the hasher is implemented as a
// singleton. IDs can be generated from a combination of source and ID string. The hasher will take
// care that all IDs will be unique.

namespace id_hash
{
template <typename ID_TYPE> // templated for the return type, to keep consecutive IDs per type
ID_TYPE get_id(std::string const &id, std::string const &source)
{
    // initialised on first call, kept alive forever :)
    static std::unordered_map<std::string, std::uint64_t> id_hash;
    const std::string concatenated_string = source + "@" + id;

    if (id_hash.count(concatenated_string))
        return ID_TYPE{id_hash.find(concatenated_string)->second};
    else
    {
        std::uint64_t new_id = id_hash.size();
        id_hash.insert(std::make_pair(concatenated_string, new_id));
        return ID_TYPE{new_id};
    }
}
} // namespace id_hash

} // namespace container
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_CONTAINER_ID_HASHER_HPP_
