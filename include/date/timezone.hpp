#ifndef NEPOMUK_DATE_TIMEZONE_HPP_
#define NEPOMUK_DATE_TIMEZONE_HPP_

#include <boost/optional.hpp>
#include <cstdint>
#include <string>

namespace nepomuk
{
namespace date
{

class Timezone
{
  public:
    // request the standard offsets of a timezone (e.g. America/Los_Angeles) in seconds. To be able
    // to use this, make sure to set-up the ICU via `icu::init` as part of `icu.hpp`
    static boost::optional<std::int32_t> offset(std::string const &timezone);
};

} // namespace date
} // namespace nepomuk

#endif // NEPOMUK_DATE_TIMEZONE_HPP_
