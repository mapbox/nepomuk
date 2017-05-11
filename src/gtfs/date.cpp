#include "gtfs/date.hpp"
#include <boost/assert.hpp>

namespace nepomuk
{
namespace gtfs
{

Date::Date() : day(0), month(0), year(0) {}

Date::Date(std::uint8_t day, std::uint8_t month, std::uint16_t year)
    : day(day), month(month), year(year)
{
}

Date::Date(std::string const &encoded_date)
    : day(std::stoi(std::string(encoded_date.begin() + 6, encoded_date.end()))),
      month(std::stoi(std::string(encoded_date.begin() + 4, encoded_date.begin() + 6))),
      year(std::stoi(std::string(encoded_date.begin(), encoded_date.begin() + 4)))
{
    BOOST_ASSERT(encoded_date.size() == 8);
}

} // namespace gtfs
} // namespace nepomuk
