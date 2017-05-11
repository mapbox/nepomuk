#ifndef NEPOMUK_DATE_TIME_HPP_
#define NEPOMUK_DATE_TIME_HPP_

#include <cstdint>
#include <iostream>

namespace nepomuk
{
namespace date
{

class Time
{
  public:
    std::uint32_t seconds_since_midnight;
    bool interpolated;

    Time();
    Time(std::uint32_t seconds_since_midnight);
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

class UTCTimestamp
{
  public:
    using value_type = std::uint32_t;
    // needs to be increased to 64_t in 2106
    UTCTimestamp(); // initialises to NOW
    UTCTimestamp(value_type seconds_since_epoch);

    // returns time in seconds
    friend value_type operator-(UTCTimestamp const &lhs, UTCTimestamp const &rhs);
    friend bool operator<(UTCTimestamp const &lhs, UTCTimestamp const &rhs);
    friend bool operator<=(UTCTimestamp const &lhs, UTCTimestamp const &rhs);
    friend bool operator==(UTCTimestamp const &lhs, UTCTimestamp const &rhs);

    // shift time by seconds
    friend UTCTimestamp operator+(UTCTimestamp lhs, value_type const seconds);
    friend UTCTimestamp operator+(value_type const seconds, UTCTimestamp lhs);
    friend UTCTimestamp operator-(UTCTimestamp lhs, value_type const seconds);

    friend std::ostream &operator<<(std::ostream &os, UTCTimestamp const &time);

    static UTCTimestamp infinity();

    value_type seconds_since_midnight_local() const;
    value_type seconds_since_midnight() const;

    value_type seconds_since_epoch;
    bool interpolated;
};

std::uint32_t operator-(UTCTimestamp const &lhs, UTCTimestamp const &rhs);
bool operator<(UTCTimestamp const &lhs, UTCTimestamp const &rhs);
bool operator<=(UTCTimestamp const &lhs, UTCTimestamp const &rhs);
bool operator==(UTCTimestamp const &lhs, UTCTimestamp const &rhs);

UTCTimestamp operator+(UTCTimestamp lhs, std::uint32_t seconds);
UTCTimestamp operator+(std::uint32_t seconds, UTCTimestamp lhs);
UTCTimestamp operator-(UTCTimestamp lhs, UTCTimestamp::value_type const seconds);

std::ostream &operator<<(std::ostream &os, UTCTimestamp const &UTCTimestamp);

} // namespace date
} // namespace nepomuk

#endif // NEPOMUK_DATE_TIME_HPP_
