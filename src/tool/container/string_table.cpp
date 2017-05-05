#include "tool/container/string_table.hpp"
#include <cstdint>

namespace transit
{
namespace tool
{
namespace container
{

std::string StringTable::get_string(DictionaryID const id) const
{
    return operator[](static_cast<std::uint64_t>(id));
}

} // namespace container
} // namespace tool
} // namespace transit
