#ifndef NEPOMUK_TOOL_IO_STREAM_CONCEPTS_LINE_STREAM_HPP_
#define NEPOMUK_TOOL_IO_STREAM_CONCEPTS_LINE_STREAM_HPP_

#include <iostream>

namespace nepomuk
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
} // namespace nepomuk

#endif // NEPOMUK_TOOL_IO_STREAM_CONCEPTS_LINE_STREAM_HPP_
