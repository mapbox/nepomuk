#ifndef NEPOMUK_TOOL_IO_CSV_DECODER_HPP_
#define NEPOMUK_TOOL_IO_CSV_DECODER_HPP_

#include "tool/io/stream_errors.hpp"
#include "tool/io/stream_traits.hpp"
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <boost/assert.hpp>
#include <boost/tokenizer.hpp>

namespace nepomuk
{
namespace tool
{
namespace io
{

// The CSVDecoder knows how to encode types into CSV
template <typename InputStream> struct CSVDecoder
{
    using tokenizer_type = boost::tokenizer<boost::escaped_list_separator<char>>;
    static_assert(IsLineInputStream<InputStream>(), "Model for LineInputStream concept required");

    CSVDecoder(InputStream &in_stream, const std::string &separators_)
        : in_stream{in_stream}, separators("\\", separators_.c_str(), "\"")
    {
        std::vector<std::string> available_headers;
        decode(available_headers);

        for (auto const &value : available_headers)
            header.insert({value, header.size()});
    }

    // For std::string we first write out the string's size and then the data
    template <typename T>
    typename std::enable_if<std::is_same<T, std::vector<std::string>>::value>::type decode(T &value)
    {
        std::string line;
        in_stream.getline(line);

        tokenizer_type tokens(line, separators);

        value.resize(std::distance(tokens.begin(), tokens.end()));

        if (!value.empty() && value.size() != header.size() && !header.empty())
            throw InvalidFileError("Token count does not match header size.");

        auto trim_special_chars = [](std::string token) {
            if (token.empty())
                return token;

            // trim spaces front
            const auto begin_non_whitespace = token.find_first_not_of(" \t\r");

            if (begin_non_whitespace == std::string::npos)
                return std::string{};

            const auto end_non_whitespace = token.find_last_not_of(" \t\r");
            return token.substr(begin_non_whitespace,
                                end_non_whitespace - begin_non_whitespace + 1);
        };

        std::transform(tokens.begin(), tokens.end(), value.begin(), trim_special_chars);
    }

    InputStream &in_stream;

    const boost::escaped_list_separator<char> separators;

    // available headers and their location
    std::map<std::string, std::size_t> header;
};

template <typename InputStream> auto makeCSVDecoder(InputStream &is, const std::string &separators)
{
    return CSVDecoder<InputStream>(is, separators);
}

} // namespace io
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_IO_BINARY_DECODER_HPP_
