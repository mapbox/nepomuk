#ifndef TRANSIT_TOOL_IO_STREAM_CONCEPTS_LINE_STREAM_HPP_
#define TRANSIT_TOOL_IO_STREAM_CONCEPTS_LINE_STREAM_HPP_

#include <cstdint>
#include <iostream>
#include <string>

namespace transit
{
namespace tool
{
namespace io
{

// Implementation of Stream Concepts for std::i/ostreams.

class StdLineInputStream
{
  public:
    StdLineInputStream(std::istream &wrapped_stream);

    void getline(std::string &line);

  private:
    std::istream &wrapped_stream;
};

class StdLineOutputStream
{
  public:
    StdLineOutputStream(std::ostream &wrapped_stream);

    void putline(const std::string &line);

  private:
    std::ostream &wrapped_stream;
};

} // namespace io
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_IO_STREAM_CONCEPTS_LINE_STREAM_HPP_
