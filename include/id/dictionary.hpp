#ifndef NEPOMUK_ID_DICTIONARY_HPP_
#define NEPOMUK_ID_DICTIONARY_HPP_

#include "tool/types_and_traits/strong_typedef.hpp"

namespace nepomuk
{
STRONG_ID_TYPE(std::uint64_t, DictionaryID)
} // namespace nepomuk

// needs to be outside of nepomuk namespace to ingest into std namespace
STRONG_ID_TYPE_HASH(std::uint64_t, nepomuk::DictionaryID)

#endif // NEPOMUK_ID_DICTIONARY_HPP_
