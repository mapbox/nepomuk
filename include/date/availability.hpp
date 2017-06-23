#ifndef NEPOMUK_DATE_AVAILABILITY_HPP_
#define NEPOMUK_DATE_AVAILABILITY_HPP_

#include <cstdint>

namespace nepomuk
{
namespace date
{

// Bit masks for all seven days of the week
namespace Weekday
{
    using Enum = std::uint8_t;
    const constexpr Enum MONDAY = 1;
    const constexpr Enum TUESDAY = 2;
    const constexpr Enum WEDNESDAY = 4;
    const constexpr Enum THURSDAY = 8;
    const constexpr Enum FRIDAY = 16;
    const constexpr Enum SATURDAY = 32;
    const constexpr Enum SUNDAY = 64;
} // namespace Weekday

// Representation of the weekly flags in GTFS data for monday through sunday
class Availability
{
  public:
    Availability(Weekday::Enum const flags);
    bool operator()(Weekday::Enum const weekday) const;

  private:
    Weekday::Enum flags;
};
} // namespace date
} // namespace nepomuk

#endif // NEPOMUK_DATE_AVAILABILITY_HPP_
