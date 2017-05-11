#include "navigation/connection.hpp"

namespace nepomuk
{
namespace navigation
{

LineID Connection::line() const { return _line; }
date::UTCTimestamp Connection::departure() const { return _departure; }
date::UTCTimestamp Connection::arrival() const { return _arrival; }
std::uint32_t Connection::duration() const { return _arrival - _departure; }

} // namespace navigation
} // namespace nepomuk
