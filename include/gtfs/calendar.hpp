#ifndef TRANSIT_GTFS_CALENDAR_HPP_
#define TRANSIT_GTFS_CALENDAR_HPP_

#include "gtfs/date.hpp"
#include "id/service.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace transit
{
namespace gtfs
{

enum class ExceptionType
{
    ADDED,
    REMOVED
};

struct CalendarDate
{
    ServiceID service_id;
    Date date;
    ExceptionType exception_type;
};

bool checkCalendarDateCSVHeader(std::map<std::string, std::size_t> const &header);
CalendarDate makeCalendarDate(std::map<std::string, std::size_t> const &header,
                              std::vector<std::string> &values);

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_CALENDAR_HPP_
