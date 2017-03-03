#ifndef TRANSIT_TIMETABLE_HPP_
#define TRANSIT_TIMETABLE_HPP_


#include "timetable/stop_table.hpp"

namespace transit
{
namespace timetable
{

class TimeTableFactory;

// The timetable class groups all the different tables together that make up the transit network. It
// essentially provides the graph and the travel times for the navigation algorithm
class TimeTable
{
    public:
        std::vector<StopTable> stop_tables;

    private:
    friend class TimeTableFactory;
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_HPP_
