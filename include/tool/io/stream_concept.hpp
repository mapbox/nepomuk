#ifndef TRANSIT_TOOL_IO_STREAM_CONCEPT_HPP_
#define TRANSIT_TOOL_IO_STREAM_CONCEPT_HPP_

#include <cstdint>
#include <iostream>

namespace transit
{
namespace tool
{
namespace io
{

// Stream Concepts - Where to read bytes from and write bytes to.
// Models for this could be a readable file, writable memory, etc.

struct InputStream
{
    void read_bytes(void *into, std::int64_t size);
};

struct OutputStream
{
    void write_bytes(const void *from, std::int64_t size);
};

} // namespace io
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_IO_STREAM_CONCEPT_HPP_
