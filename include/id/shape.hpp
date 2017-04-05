#ifndef TRANSIT_ID_SHAPE_HPP_
#define TRANSIT_ID_SHAPE_HPP_

#include "tool/types_and_traits/strong_typedef.hpp"

namespace transit
{
STRONG_ID_TYPE(std::uint64_t, ShapeID)
} // namespace transit

// needs to be outside of transit namespace to ingest into std namespace
STRONG_ID_TYPE_HASH(std::uint64_t, transit::ShapeID)

#endif // TRANSIT_ID_SHAPE_HPP_
