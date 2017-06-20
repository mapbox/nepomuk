#ifndef NEPOMUK_TOOL_IO_STRING_HPP_
#define NEPOMUK_TOOL_IO_STRING_HPP_

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>

namespace nepomuk
{
namespace tool
{
namespace io
{

inline bool requires_escape(char const letter)
{
    auto const is_quote = letter == '\"';
    auto const is_slash = letter == '\\';
    auto const is_backslash = letter == '/';
    return is_quote || is_slash || is_backslash;
};

inline std::string to_escaped(std::string const &unescaped)
{
    std::ostringstream oss;

    // escape all chars, if necessary
    auto escape_char = [&](char const letter) {
        if (iscntrl(letter) || requires_escape(letter))
            oss << '\\';

        oss << letter;
    };

    std::for_each(unescaped.begin(), unescaped.end(), escape_char);

    return oss.str();
};

} // namespace io
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_IO_STRING_HPP_
