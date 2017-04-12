#ifndef TRANSIT_DATE_TIME_HPP_
#define TRANSIT_DATE_TIME_HPP_

#include <cstdint>
#include <iostream>
#include <string>

namespace transit
{
namespace date
{

class Time
{
  public:
    std::uint32_t seconds_since_midnight;
    bool interpolated;

    Time();
    Time(std::uint8_t hour, std::uint8_t minute, std::uint8_t second);
    Time(std::string const &);

    // returns time in seconds
    friend std::uint32_t operator-(Time const &lhs, Time const &rhs);
    friend bool operator<(Time const &lhs, Time const &rhs);
    friend bool operator<=(Time const &lhs, Time const &rhs);
    friend bool operator==(Time const &lhs, Time const &rhs);

    // shift time by seconds
    friend Time operator+(Time lhs, std::uint32_t seconds);
    friend Time operator+(std::uint32_t seconds, Time lhs);
    friend Time operator-(Time lhs, std::uint32_t seconds);

    friend std::ostream &operator<<(std::ostream &os, Time const &time);

    static Time infinity();
};

std::uint32_t operator-(Time const &lhs, Time const &rhs);
bool operator<(Time const &lhs, Time const &rhs);
bool operator<=(Time const &lhs, Time const &rhs);
bool operator==(Time const &lhs, Time const &rhs);

Time operator+(Time lhs, std::uint32_t seconds);
Time operator+(std::uint32_t seconds, Time lhs);

std::ostream &operator<<(std::ostream &os, Time const &time);

} // namespace date
} // namespace transit

#endif // TRANSIT_DATE_TIME_HPP_
