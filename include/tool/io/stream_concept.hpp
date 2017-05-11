#ifndef NEPOMUK_TOOL_IO_STREAM_CONCEPT_HPP_
#define NEPOMUK_TOOL_IO_STREAM_CONCEPT_HPP_

#include <cstdint>
#include <iostream>
#include <string>

namespace nepomuk
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

struct LineInputStream
{
    void getline(std::string &line);
};

struct LineOutputStream
{
    void putline(const std::string &line);
};

} // namespace io
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_IO_STREAM_CONCEPT_HPP_
