#ifndef TRANSIT_TIMETABLE_FACTORY_HPP_
#define TRANSIT_TIMETABLE_FACTORY_HPP_

#include "timetable/timetable.hpp"
#include "gtfs/dataset.hpp"

namespace transit
{
namespace timetable
{

class TimeTableFactory
{
    public:
    TimeTable produce(gtfs::Dataset const& dataset);
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_FACTORY_HPP_
