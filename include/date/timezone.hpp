#ifndef NEPOMUK_DATE_TIMEZONE_HPP_
#define NEPOMUK_DATE_TIMEZONE_HPP_

#include <cstdint>
#include <string>
#include <boost/optional.hpp>

namespace nepomuk
{
namespace date
{

class Timezone
{
    public:
    static boost::optional<std::int32_t> offset(std::string const& timezone);
};

} // namespace date
} // namespace nepomuk

#endif // NEPOMUK_DATE_TIMEZONE_HPP_
