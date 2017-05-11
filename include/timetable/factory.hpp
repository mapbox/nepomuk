#ifndef NEPOMUK_TIMETABLE_FACTORY_HPP_
#define NEPOMUK_TIMETABLE_FACTORY_HPP_

#include "gtfs/dataset.hpp"
#include "timetable/timetable.hpp"

namespace nepomuk
{
namespace timetable
{

class TimeTableFactory
{
  public:
    TimeTable produce(gtfs::Dataset const &dataset);
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_FACTORY_HPP_
