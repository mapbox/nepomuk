#ifndef NEPOMUK_TIMETABLE_EXCEPTIONS_HPP_
#define NEPOMUK_TIMETABLE_EXCEPTIONS_HPP_

#include <stdexcept>

namespace nepomuk
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
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_EXCEPTIONS_HPP_
