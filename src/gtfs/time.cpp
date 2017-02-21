#include "gtfs/time.hpp"

#include <boost/assert.hpp>

namespace transit
{
namespace gtfs
{

Time::Time() : hour(0), minute(0), second(0) {}
Time::Time(std::uint8_t hour, std::uint8_t minute, std::uint8_t second)
    : hour(hour), minute(minute), second(second)
{
}

Time::Time(std::string const &encoded_time)
{
    const auto end_hours_pos = encoded_time.find_first_of(':');
    BOOST_ASSERT(end_hours_pos != std::string::npos);
    const auto end_hours = encoded_time.begin() + end_hours_pos;
    hour = std::stoi(std::string(encoded_time.begin(), end_hours));
    minute = std::stoi(std::string(end_hours + 1, end_hours + 3));
    second = std::stoi(std::string(end_hours + 4, encoded_time.end()));
}

} // namespace gtfs
} // namespace transit
