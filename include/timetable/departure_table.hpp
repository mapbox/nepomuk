#ifndef TRANSIT_TIMETABLE_DEPARTURETABLE_HPP_
#define TRANSIT_TIMETABLE_DEPARTURETABLE_HPP_

namespace transit
{
namespace timetable
{

// Public transit routing consists of timetable routing. The departuretables define when connections leave certain stops. In
// general these occurr in certain periodic settings. For any time at any stop we need to be able to access the next
// connections that are possible. The departuretable itself assumes the stop, line, and the time of arrival are known and allows
// quick access to the next connection of the given departure. It only represents a single line.
class DepartureTable
{
    // give access to the next departure of the given departure
    void next(/*time*/);
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_DEPARTURETABLE_HPP_
