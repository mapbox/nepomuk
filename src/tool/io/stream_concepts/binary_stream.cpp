#include "tool/io/stream_concepts/binary_stream.hpp"

namespace transit
{
namespace tool
{
namespace io
{

// Stream Concepts - Where to read bytes from and write bytes to.
// Models for this could be a readable file, writable memory, etc.

void BinaryInputStream::ReadBytes(void *into, std::int64_t size)
{
}

void BinaryOutputStream::WriteBytes(const void *from, std::int64_t size)
{
}

} // namespace io
} // namespace tool
} // namespace transit
