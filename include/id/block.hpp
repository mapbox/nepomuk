#ifndef TRANSIT_ID_BLOCK_HPP_
#define TRANSIT_ID_BLOCK_HPP_

#include "tool/types_and_traits/strong_typedef.hpp"

namespace transit
{
    STRONG_ID_TYPE(std::uint64_t, BlockID)
} // namespace transit

// needs to be outside of transit namespace to ingest into std namespace
STRONG_ID_TYPE_HASH(std::uint64_t, transit::BlockID)

#endif //TRANSIT_ID_BLOCK_HPP_
