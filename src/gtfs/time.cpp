#include "gtfs/time.hpp"

#include <boost/assert.hpp>
#include <tuple>

namespace transit
{
namespace gtfs
{

Time::Time() : seconds_since_midnight(0), interpolated(false) {}
Time::Time(std::uint8_t hour, std::uint8_t minute, std::uint8_t second) : interpolated(false)
{
    seconds_since_midnight = hour;
    seconds_since_midnight *= 60;
    seconds_since_midnight += minute;
    seconds_since_midnight *= 60;
    seconds_since_midnight += second;
}

Time::Time(std::string const &encoded_time)
{
    if (encoded_time.empty())
    {
        interpolated = true;
        seconds_since_midnight = 0;
    }
    else
    {
        interpolated = false;
        const auto end_hours_pos = encoded_time.find_first_of(':');
        BOOST_ASSERT(end_hours_pos != std::string::npos);
        const auto end_hours = encoded_time.begin() + end_hours_pos;
        seconds_since_midnight = std::stoi(std::string(encoded_time.begin(), end_hours));
        seconds_since_midnight *= 60;
        seconds_since_midnight += std::stoi(std::string(end_hours + 1, end_hours + 3));
        seconds_since_midnight *= 60;
        seconds_since_midnight += std::stoi(std::string(end_hours + 4, encoded_time.end()));
    }
}

std::uint32_t operator-(Time const &lhs, Time const &rhs)
{
    return lhs.seconds_since_midnight - rhs.seconds_since_midnight;
}

bool operator<(Time const &lhs, Time const &rhs)
{
    return lhs.seconds_since_midnight < rhs.seconds_since_midnight;
}

bool operator<=(Time const &lhs, Time const &rhs)
{
    return lhs.seconds_since_midnight <= rhs.seconds_since_midnight;
}

bool operator==(Time const &lhs, Time const &rhs)
{
    return lhs.seconds_since_midnight == rhs.seconds_since_midnight;
}

Time operator+(Time lhs, std::uint32_t seconds)
{
    lhs.seconds_since_midnight += seconds;
    return lhs;
}
Time operator+(std::uint32_t seconds, Time rhs)
{
    rhs.seconds_since_midnight += seconds;
    return rhs;
}

std::ostream &operator<<(std::ostream &os, Time const &time)
{
    auto hours = time.seconds_since_midnight / 3600;
    auto minutes = (time.seconds_since_midnight % 3600) / 60;
    auto seconds = time.seconds_since_midnight % 60;

    os << hours << ":" << (minutes < 10 ? "0" : "") << minutes << ":" << (seconds < 10 ? "0" : "")
       << seconds;
    return os;
}

} // namespace gtfs
} // namespace transit
