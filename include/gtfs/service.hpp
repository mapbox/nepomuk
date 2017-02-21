#ifndef TRANSIT_GTFS_SERVICE_HPP_
#define TRANSIT_GTFS_SERVICE_HPP_

#include <cstdint>

#include "tool/types_and_traits/strong_typedef.hpp"

namespace transit
{
namespace gtfs
{

STRONG_ID_TYPE(std::uint64_t, ServiceID)

} // namespace gtfs
} // namespace transit

// needs to be outside of namespaces
STRONG_ID_TYPE_HASH(std::uint64_t, transit::gtfs::ServiceID)
#endif // TRANSIT_GTFS_SERVICE_HPP_
