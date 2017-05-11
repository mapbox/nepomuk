#ifndef NEPOMUK_TOOL_IO_PROTOCOL_HPP_
#define NEPOMUK_TOOL_IO_PROTOCOL_HPP_

namespace nepomuk
{
namespace tool
{
namespace io
{

// Protocol Concepts - How to encode from types to bytes and decode from bytes
// to types. This could be a binary encoding / decoding, or e.g. using Cap'n
// Proto.

template <typename OutputStream> struct Encoder
{
    Encoder(OutputStream &outStream);

    template <typename T> void encode(const T &value);
};

template <typename InputStream> struct Decoder
{
    Decoder(InputStream &inStream);

    template <typename T> void decode(T &into);
};

} // namespace io
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_IO_PROTOCOL_HPP_
