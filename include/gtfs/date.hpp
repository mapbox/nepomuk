#ifndef TRANSIT_GTFS_DATE_HPP_
#define TRANSIT_GTFS_DATE_HPP_

#include <cstdint>
#include <string>

namespace transit
{
namespace gtfs
{

struct Date
{
    std::uint8_t day;
    std::uint8_t month;
    std::uint16_t year;

    Date();
    Date(std::uint8_t day, std::uint8_t month, std::uint16_t year);
    Date(std::string const &encoded_date);
};

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_DATE_HPP_
