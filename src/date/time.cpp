#include "date/time.hpp"

#include <boost/assert.hpp>
#include <ctime>
#include <iomanip>
#include <limits>
#include <string>

namespace nepomuk
{
namespace date
{

Time::Time() : seconds_since_midnight(0), interpolated(false) {}

Time::Time(std::uint32_t seconds_since_midnight) : seconds_since_midnight(seconds_since_midnight) {}

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
Time operator-(Time lhs, std::uint32_t seconds)
{
    BOOST_ASSERT(seconds <= lhs.seconds_since_midnight);
    lhs.seconds_since_midnight -= seconds;
    return lhs;
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

Time Time::infinity()
{
    Time time;
    time.seconds_since_midnight = std::numeric_limits<std::uint32_t>::max();
    return time;
}

//////////////////

UTCTimestamp::UTCTimestamp() : seconds_since_epoch(0), interpolated(false) {}
UTCTimestamp::UTCTimestamp(value_type seconds_since_epoch)
    : seconds_since_epoch(seconds_since_epoch)
{
}

UTCTimestamp::value_type operator-(UTCTimestamp const &lhs, UTCTimestamp const &rhs)
{
    return lhs.seconds_since_epoch - rhs.seconds_since_epoch;
}

bool operator<(UTCTimestamp const &lhs, UTCTimestamp const &rhs)
{
    return lhs.seconds_since_epoch < rhs.seconds_since_epoch;
}

bool operator<=(UTCTimestamp const &lhs, UTCTimestamp const &rhs)
{
    return lhs.seconds_since_epoch <= rhs.seconds_since_epoch;
}

bool operator==(UTCTimestamp const &lhs, UTCTimestamp const &rhs)
{
    return lhs.seconds_since_epoch == rhs.seconds_since_epoch;
}

UTCTimestamp operator+(UTCTimestamp lhs, UTCTimestamp::value_type const seconds)
{
    lhs.seconds_since_epoch += seconds;
    return lhs;
}
UTCTimestamp operator+(UTCTimestamp::value_type const seconds, UTCTimestamp rhs)
{
    rhs.seconds_since_epoch += seconds;
    return rhs;
}
UTCTimestamp operator-(UTCTimestamp lhs, UTCTimestamp::value_type const seconds)
{
    BOOST_ASSERT(seconds <= lhs.seconds_since_epoch);
    lhs.seconds_since_epoch -= seconds;
    return lhs;
}

std::ostream &operator<<(std::ostream &os, UTCTimestamp const &time)
{
    std::time_t when = time.seconds_since_epoch;
    auto const gm_time = std::gmtime(&when);
    os << std::put_time(gm_time, "%c");
    return os;
}

UTCTimestamp UTCTimestamp::infinity()
{
    UTCTimestamp time;
    time.seconds_since_epoch = std::numeric_limits<value_type>::max();
    return time;
}

UTCTimestamp::value_type UTCTimestamp::seconds_since_midnight_local() const
{
    std::time_t when = seconds_since_epoch;
    auto const local_time = std::localtime(&when);
    return 24 * 60 * local_time->tm_hour + 60 * local_time->tm_min + local_time->tm_sec;
}

UTCTimestamp::value_type UTCTimestamp::seconds_since_midnight() const
{
    std::time_t when = seconds_since_epoch;
    auto const local_time = std::gmtime(&when);
    return 60 * 60 * local_time->tm_hour + 60 * local_time->tm_min + local_time->tm_sec;
}

} // namespace date
} // namespace nepomuk
