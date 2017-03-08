#ifndef TRANSIT_GTFS_TIME_HPP_
#define TRANSIT_GTFS_TIME_HPP_

#include <cstdint>
#include <string>
#include <iostream>

namespace transit
{
namespace gtfs
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

    friend std::ostream& operator<<(std::ostream &os, Time const& time);
};

std::uint32_t operator-(Time const &lhs, Time const &rhs);
bool operator<(Time const &lhs, Time const &rhs);
bool operator<=(Time const &lhs, Time const &rhs);
bool operator==(Time const &lhs, Time const &rhs);

Time operator+(Time lhs, std::uint32_t seconds);
Time operator+(std::uint32_t seconds, Time lhs);

std::ostream& operator<<(std::ostream &os, Time const& time);

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_TIME_HPP_
