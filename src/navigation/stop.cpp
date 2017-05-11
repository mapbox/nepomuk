#include "navigation/stop.hpp"

namespace nepomuk
{
namespace navigation
{

StopID Stop::id() const { return _id; }
date::UTCTimestamp Stop::departure() const { return _departure; }
date::UTCTimestamp Stop::arrival() const { return _arrival; }

} // namespace navigation
} // namespace nepomuk
