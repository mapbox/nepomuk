#ifndef TRANSIT_TIMETABLE_EXCEPTIONS_HPP_
#define TRANSIT_TIMETABLE_EXCEPTIONS_HPP_

#include <stdexcept>

namespace transit
{
namespace timetable
{

// Error handling for timetables
struct TimeTableError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

// Ensure our input is correct
struct InvalidInputError : TimeTableError
{
    using TimeTableError::TimeTableError;
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_EXCEPTIONS_HPP_
