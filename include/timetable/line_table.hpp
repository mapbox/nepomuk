#ifndef TRANSIT_TIMETABLE_LINETABLE_HPP_
#define TRANSIT_TIMETABLE_LINETABLE_HPP_

namespace transit
{
namespace timetable
{

// Public transit routing consists of timetable routing. The linetables define which connections leave certain stops.
// It provides access to all possible lines.
class LineTable
{
    // give access to the next departure of the given line
    void list(/*stop*/);
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_LINETABLE_HPP_
