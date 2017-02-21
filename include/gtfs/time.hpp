#ifndef TRANSIT_GTFS_TIME_HPP_
#define TRANSIT_GTFS_TIME_HPP_

#include <cstdint>
#include <string>

namespace transit
{
namespace gtfs
{

struct Time
{
    std::uint8_t hour; // at most 256 hours (10.5 day trips)
    std::uint8_t minute;
    std::uint8_t second;

    Time();
    Time(std::uint8_t hour, std::uint8_t minute, std::uint8_t second);
    Time(std::string const &);
};

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_TIME_HPP_
