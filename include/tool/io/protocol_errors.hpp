#ifndef NEPOMUK_TOOL_IO_PROTOCOL_ERRORS_HPP_
#define NEPOMUK_TOOL_IO_PROTOCOL_ERRORS_HPP_

#include <stdexcept>

namespace nepomuk
{
namespace tool
{
namespace io
{

// Error Handling for Reader / Writer

struct ProtocolError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct EncodeError : ProtocolError
{
    using ProtocolError::ProtocolError;
};

struct DecodeError : ProtocolError
{
    using ProtocolError::ProtocolError;
};

} // namespace io
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_IO_PROTOCOL_ERRORS_HPP_
