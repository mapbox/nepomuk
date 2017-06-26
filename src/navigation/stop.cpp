#include "navigation/stop.hpp"

namespace nepomuk
{
namespace navigation
{

StopID Stop::id() const { return _id; }
std::size_t Stop::offset() const { return _offset; }
date::UTCTimestamp Stop::departure() const { return _departure; }
date::UTCTimestamp Stop::arrival() const { return _arrival; }

} // namespace navigation
} // namespace nepomuk
