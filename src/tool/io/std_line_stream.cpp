#include "tool/io/std_line_stream.hpp"
#include "tool/io/stream_errors.hpp"

#include <string>

namespace transit
{
namespace tool
{
namespace io
{

// Stream Concepts - Where to read bytes from and write bytes to.
// Models for this could be a readable file, writable memory, etc.

StdLineInputStream::StdLineInputStream(std::istream &wrapped_stream)
    : wrapped_stream(wrapped_stream)
{
}

void StdLineInputStream::getline(std::string &line)
{
    std::getline(wrapped_stream, line);

    if (wrapped_stream.eof())
        return;

    if (!wrapped_stream)
        throw InputStreamError("Failed to read new line from stream");
}

StdLineOutputStream::StdLineOutputStream(std::ostream &wrapped_stream)
    : wrapped_stream(wrapped_stream)
{
}

void StdLineOutputStream::putline(const std::string &line)
{
    if (!line.empty() && (line.back() == '\n' || line.back() == '\r'))
        throw OutputStreamError("Invalid character (newline) during encoding");

    // output a full line, followed by a newline
    wrapped_stream << line << "\n";

    if (!wrapped_stream)
        throw OutputStreamError("Failed to output line into stream.");
}

} // namespace io
} // namespace tool
} // namespace transit
