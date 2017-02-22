#ifndef TRANSIT_TOOL_IO_STREAM_CONCEPTS_BINDARY_STREAM_HPP_
#define TRANSIT_TOOL_IO_STREAM_CONCEPTS_BINDARY_STREAM_HPP_

#include <cstdint>

namespace transit {
namespace tool {
namespace io {

// Stream Concepts - Where to read bytes from and write bytes to.
// Models for this could be a readable file, writable memory, etc.

struct BinaryInputStream {
    void ReadBytes(void* into, std::int64_t size);
};

struct BinaryOutputStream {
    void WriteBytes(const void* from, std::int64_t size);
};

}  // namespace io
}  // namespace tool
}  // namespace transit

#endif  // TRANSIT_TOOL_IO_STREAM_CONCEPTS_BINDARY_STREAM_HPP_
