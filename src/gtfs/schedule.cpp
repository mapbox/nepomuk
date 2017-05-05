#include "gtfs/schedule.hpp"
#include "gtfs/constructor.hpp"

#include <algorithm>
#include <boost/assert.hpp>
#include <utility>

namespace transit
{
namespace gtfs
{
bool checkWeeklyScheduleCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("service_id") && header.count("monday") && header.count("tuesday") &&
           header.count("wednesday") && header.count("thursday") && header.count("friday") &&
           header.count("saturday") && header.count("sunday") && header.count("start_date") &&
           header.count("end_date");
}

WeeklySchedule makeWeeklySchedule(std::map<std::string, std::size_t> const &header,
                                  std::vector<std::string> &values)
{
    std::bitset<7> schedule;
    auto const is_set = [](std::string const &value) {
        BOOST_ASSERT(value == "0" || value == "1");
        return value == "1";
    };
    const constexpr char *weekdays[7] = {
        "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};
    for (std::size_t i = 0; i < 7; ++i)
    {
        schedule[i] = is_set(construct<std::string>(weekdays[i], forward, header, values));
    }

    return {construct<ServiceID>("service_id", stringToID<ServiceID>, header, values),
            std::move(schedule),
            construct<Date>("start_date", constructFromString<Date>, header, values),
            construct<Date>("end_date", constructFromString<Date>, header, values)};
}
} // namespace gtfs
} // namespace transit
