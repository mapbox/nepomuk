#ifndef NEPOMUK_TOOL_IO_STREAM_ERRORS_HPP_
#define NEPOMUK_TOOL_IO_STREAM_ERRORS_HPP_

#include <stdexcept>

namespace nepomuk
{
namespace tool
{
namespace io
{

// Error Handling for Streams
struct StreamError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct InputStreamError : StreamError
{
    using StreamError::StreamError;
};

struct OutputStreamError : StreamError
{
    using StreamError::StreamError;
};

struct FileNotFoundError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct InvalidFileError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

} // namespace io
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_IO_STREAM_ERRORS_HPP_
