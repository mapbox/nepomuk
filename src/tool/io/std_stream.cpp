#include "tool/io/std_stream.hpp"
#include "tool/io/stream_errors.hpp"

#include <boost/assert.hpp>
#include <string>

namespace nepomuk
{
namespace tool
{
namespace io
{

// Stream Concepts - Where to read bytes from and write bytes to.
// Models for this could be a readable file, writable memory, etc.

StdInputStream::StdInputStream(std::istream &wrapped_stream) : wrapped_stream(wrapped_stream)
{
    BOOST_ASSERT(wrapped_stream);
}

void StdInputStream::read_bytes(void *into, std::int64_t size)
{
    if (!into || size == 0)
        return;

    wrapped_stream.read(static_cast<char *>(into), size);

    if (!wrapped_stream)
        throw InputStreamError("Failed to read " + std::to_string(size) + " bytes.");
}

StdOutputStream::StdOutputStream(std::ostream &wrapped_stream) : wrapped_stream(wrapped_stream)
{
    BOOST_ASSERT(wrapped_stream);
}

void StdOutputStream::write_bytes(const void *from, std::int64_t size)
{
    if (!from || size == 0)
        return;

    wrapped_stream.write(static_cast<const char *>(from), size);

    if (!wrapped_stream)
        throw OutputStreamError("Failed to write " + std::to_string(size) + " bytes.");
}

} // namespace io
} // namespace tool
} // namespace nepomuk
