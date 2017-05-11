#ifndef NEPOMUK_GTFS_DATE_HPP_
#define NEPOMUK_GTFS_DATE_HPP_

#include <cstdint>
#include <string>

namespace nepomuk
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
} // namespace nepomuk

#endif // NEPOMUK_GTFS_DATE_HPP_
