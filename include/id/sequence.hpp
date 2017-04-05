#ifndef TRANSIT_ID_SEQUENCE_HPP_
#define TRANSIT_ID_SEQUENCE_HPP_

#include "tool/types_and_traits/strong_typedef.hpp"

namespace transit
{
STRONG_ID_TYPE(std::uint64_t, SequenceID)
} // namespace transit

// needs to be outside of transit namespace to ingest into std namespace
STRONG_ID_TYPE_HASH(std::uint64_t, transit::SequenceID)

#endif // TRANSIT_ID_SEQUENCE_HPP_
