#ifndef NEPOMUK_TOOL_CONTAINER_STRING_TABLE_HPP_
#define NEPOMUK_TOOL_CONTAINER_STRING_TABLE_HPP_

#include <iosfwd>
#include <vector>

#include "id/dictionary.hpp"

namespace nepomuk
{

namespace adaptor
{
// I am really forward here, but I want to become friends
class Dictionary;
} // namespace adaptor

namespace tool
{
namespace container
{

// wrapper for look-up from dictionary ID to string
class StringTable : private std::vector<std::string>
{
    friend class adaptor::Dictionary;

  public:
    std::string get_string(DictionaryID const id) const;
};

} // namespace container
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_CONTAINER_STRING_TABLE_HPP_
