#ifndef TRANSIT_GTFS_SCHEDULE_HPP_
#define TRANSIT_GTFS_SCHEDULE_HPP_

#include <bitset>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "gtfs/date.hpp"
#include "gtfs/service.hpp"

namespace transit
{
namespace gtfs
{

enum class Weekday
{
    MONDAY = 0,
    TUESDAY = 1,
    WEDNESDAY = 2,
    THURSDAY = 3,
    FRIDAY = 4,
    SATURADY = 5,
    SUNDAY = 6
};

struct WeeklySchedule
{
    ServiceID service_id;

    // availability from monday through sunday
    std::bitset<7> availability;

    Date begin;
    Date end;
};

bool checkWeeklyScheduleCSVHeader(std::map<std::string, std::size_t> const &header);
WeeklySchedule makeWeeklySchedule(std::map<std::string, std::size_t> const &header,
                                  std::vector<std::string> &values);

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_SCHEDULE_HPP_
