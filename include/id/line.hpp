#ifndef TRANSIT_ID_LINE_HPP_
#define TRANSIT_ID_LINE_HPP_

#include "tool/types_and_traits/strong_typedef.hpp"

namespace transit
{
STRONG_ID_TYPE(std::uint64_t, LineID)

auto const constexpr WALKING_TRANSFER = LineID{static_cast<std::uint64_t>(-1)};
} // namespace transit

// needs to be outside of transit namespace to ingest into std namespace
STRONG_ID_TYPE_HASH(std::uint64_t, transit::LineID)

#endif // TRANSIT_ID_LINE_HPP_
