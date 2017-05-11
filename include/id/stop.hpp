#ifndef NEPOMUK_ID_STOP_HPP_
#define NEPOMUK_ID_STOP_HPP_

#include "tool/types_and_traits/strong_typedef.hpp"

namespace nepomuk
{
STRONG_ID_TYPE(std::uint64_t, StopID)
} // namespace nepomuk

// needs to be outside of nepomuk namespace to ingest into std namespace
STRONG_ID_TYPE_HASH(std::uint64_t, nepomuk::StopID)

#endif // NEPOMUK_ID_STOP_HPP_
