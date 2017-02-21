#ifndef TRANSIT_TOOL_IO_SERIALISATION_HPP_
#define TRANSIT_TOOL_IO_SERIALISATION_HPP_

#include "tool/io/protocol_traits.hpp"

namespace transit
{
namespace tool
{
namespace io
{

// Serialize and Deserialize functions for type deduction: no need to specify T or Protocol.
// Usage: Serialize(object, encoder);

template <typename T, typename Protocol> void Serialize(const T &value, Protocol &protocol)
{
    static_assert(IsEncodeable<Protocol, T>(), "Unable to encode T with this Protocol");
    protocol.encode(value);
}

template <typename T, typename Protocol> void Deserialize(T &value, Protocol &protocol)
{
    static_assert(IsDecodeable<Protocol, T>(), "Unable to decode T with this Protocol");
    protocol.decode(value);
}

} // namespace io
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_IO_SERIALISATION_HPP_
