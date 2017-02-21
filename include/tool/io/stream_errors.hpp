#ifndef TRANSIT_TOOL_IO_STREAM_ERRORS_HPP_
#define TRANSIT_TOOL_IO_STREAM_ERRORS_HPP_

#include <stdexcept>

namespace transit {
namespace tool {
namespace io {

// Error Handling for Streams
struct StreamError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct InputStreamError : StreamError {
    using StreamError::StreamError;
};

struct OutputStreamError : StreamError {
    using StreamError::StreamError;
};

}  // namespace io
}  // namespace tool
}  // namespace transit

#endif  // TRANSIT_TOOL_IO_STREAM_ERRORS_HPP_
