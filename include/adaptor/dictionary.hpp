#ifndef NEPOMUK_ADAPTOR_DICTIONARY_HPP
#define NEPOMUK_ADAPTOR_DICTIONARY_HPP

#include "dictionary.pb.h"

namespace nepomuk
{

// Forward declarations

namespace tool
{
namespace container
{
class Dictionary;
class StringTable;
} // namespace container
} // namespace tool

namespace adaptor
{

class Dictionary
{
  public:
    // transforms a dictionary into a respective dictionary message in PBF format (see
    // tool/proto/dictionary.proto). The message can be used to serialise it to a file/send the
    // message via network
    static io::container::Dictionary_proto encode(tool::container::Dictionary const &dictionary);

    static void decode_into(tool::container::Dictionary &dictionary,
                            io::container::Dictionary_proto const &message);
    static void decode_into(tool::container::StringTable &dictionary,
                            io::container::Dictionary_proto const &message);
};

} // namespace adaptor
} // namespace nepomuk

#endif // NEPOMUK_ADAPTOR_DICTIONARY_HPP
