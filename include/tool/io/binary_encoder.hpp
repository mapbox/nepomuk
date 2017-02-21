#ifndef TRANSIT_TOOL_IO_BINARY_ENCODER_HPP_
#define TRANSIT_TOOL_IO_BINARY_ENCODER_HPP_

#include "tool/io/stream_traits.hpp"

namespace transit
{
namespace tool
{
namespace io
{

// The BinaryEncoder knows how to encode types into binary
template <typename OutputStream> struct BinaryEncoder
{
    static_assert(IsOutputStream<OutputStream>(), "Model for OutputStream concept required");

    BinaryEncoder(OutputStream &outStream) : outStream{outStream} {}

    // For all memcpy-able types we dump byte-wise
    template <typename T>
    typename std::enable_if<std::is_trivially_copyable<T>::value>::type encode(const T &value)
    {
        outStream.WriteBytes(&value, sizeof(T));
    }

    // For std::string we first write out the string's size and then the data
    template <typename T>
    typename std::enable_if<std::is_same<T, std::string>::value>::type encode(const T &value)
    {
        auto len = (std::uint64_t)value.size();
        encode(len);

        outStream.WriteBytes(value.data(), value.size());
    }

    OutputStream &outStream;
};

} // namespace io
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_IO_BINARY_ENCODER_HPP_
