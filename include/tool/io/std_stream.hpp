#ifndef TRANSIT_TOOL_IO_STREAM_CONCEPTS_BINARY_STREAM_HPP_
#define TRANSIT_TOOL_IO_STREAM_CONCEPTS_BINARY_STREAM_HPP_

#include <iostream>

namespace transit
{
namespace tool
{
namespace io
{

// Implementation of Stream Concepts for std::i/ostreams.

class StdInputStream
{
  public:
    StdInputStream(std::istream &wrapped_stream);

    void read_bytes(void *into, std::int64_t size);

  private:
    std::istream &wrapped_stream;
};

class StdOutputStream
{
  public:
    StdOutputStream(std::ostream &wrapped_stream);

    void write_bytes(const void *from, std::int64_t size);

  private:
    std::ostream &wrapped_stream;
};

} // namespace io
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_IO_STREAM_CONCEPTS_BINARY_STREAM_HPP_
