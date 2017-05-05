#include "gtfs/calendar.hpp"
#include "gtfs/constructor.hpp"

#include <boost/assert.hpp>

namespace transit
{
namespace gtfs
{

bool checkCalendarDateCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("service_id") && header.count("date") && header.count("exception_type");
}

CalendarDate makeCalendarDate(std::map<std::string, std::size_t> const &header,
                              std::vector<std::string> &values)
{
    auto const to_exception_type = [](std::string value) {
        if (value == "1")
            return ExceptionType::ADDED;
        else
        {
            BOOST_ASSERT(value == "2");
            return ExceptionType::REMOVED;
        }
    };

    return {construct<ServiceID>("service_id", stringToID<ServiceID>, header, values),
            construct<Date>("date", constructFromString<Date>, header, values),
            construct<ExceptionType>("exception_type", to_exception_type, header, values)};
}

} // namespace gtfs
} // namespace transit
