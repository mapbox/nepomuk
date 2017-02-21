#ifndef TRANSIT_TOOL_IO_PROTOCOL_HPP_
#define TRANSIT_TOOL_IO_PROTOCOL_HPP_

#include <type_traits>

namespace transit {
namespace tool {
namespace io {

// Compile time protocol concept checks.
// Usage example: static_assert(IsEncodeable<BinaryEncoder, uint32_t>(), "");

template <typename Protocol, typename T, typename = void>
struct IsEncodeable : std::false_type {};

template <typename Protocol, typename T>
struct IsEncodeable<Protocol, T, decltype((void)std::declval<Protocol>().encode(std::declval<T>()))> : std::true_type {};

template <typename Protocol, typename T, typename = void>
struct IsDecodeable : std::false_type {};

template <typename Protocol, typename T>
struct IsDecodeable<Protocol, T, decltype((void)std::declval<Protocol>().decode(std::declval<T&>()))> : std::true_type {};

template <typename Protocol, typename T>
struct IsEncodeableDecodeable : std::integral_constant<bool, IsEncodeable<Protocol, T>() && IsDecodeable<Protocol, T>()> {};

}  // namespace io
}  // namespace tool
}  // namespace transit

#endif  // TRANSIT_TOOL_IO_PROTOCOL_HPP_
