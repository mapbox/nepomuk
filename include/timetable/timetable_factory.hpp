#ifndef NEPOMUK_TIMETABLE_FACTORY_HPP_
#define NEPOMUK_TIMETABLE_FACTORY_HPP_

#include <vector>

#include "timetable/timetable.hpp"

namespace nepomuk
{

struct ShapeID;
struct TripID;
namespace gtfs
{
struct Dataset;
}

namespace timetable
{

class TimeTableFactory
{
  public:
    static TimeTable
    produce(gtfs::Dataset &dataset,
            // if required, we can remember an output mpaaing from internal to external trip ids
            std::vector<TripID> *trip_mapping = NULL);
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_FACTORY_HPP__
