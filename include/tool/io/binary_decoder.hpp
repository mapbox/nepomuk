#ifndef TRANSIT_TOOL_IO_BINARY_DECODER_HPP_
#define TRANSIT_TOOL_IO_BINARY_DECODER_HPP_

#include "tool/io/stream_traits.hpp"
#include <cstdint>
#include <string>

namespace transit
{
namespace tool
{
namespace io
{

// The BinaryDecoder knows how to encode types into binary
template <typename InputStream> struct BinaryDecoder
{
    static_assert(IsInputStream<InputStream>(), "Model for InputStream concept required");

    BinaryDecoder(InputStream &in_stream) : in_stream{in_stream} {}

    // For all memcpy-able types we dump byte-wise
    template <typename T>
    typename std::enable_if<std::is_trivially_copyable<T>::value>::type decode(T &value)
    {
        in_stream.read_bytes(&value, sizeof(T));
    }

    // For std::string we first write out the string's size and then the data
    template <typename T>
    typename std::enable_if<std::is_same<T, std::string>::value>::type decode(T &value)
    {
        std::uint64_t length;
        decode(length);

        value.resize(length);
        in_stream.read_bytes(&value[0], value.size());
    }

    InputStream &in_stream;
};

template <typename InputStream> auto makeBinaryDecoder(InputStream &is)
{
    return BinaryDecoder<InputStream>(is);
}

} // namespace io
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_IO_BINARY_DECODER_HPP_
