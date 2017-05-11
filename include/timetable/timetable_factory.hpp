#ifndef NEPOMUK_TIMETABLE_FACTORY_HPP_
#define NEPOMUK_TIMETABLE_FACTORY_HPP_

#include <boost/optional.hpp>
#include <vector>

#include "timetable/timetable.hpp"

namespace nepomuk
{

struct ShapeID;
namespace gtfs
{
struct Dataset;
}

namespace timetable
{

class TimeTableFactory
{
  public:
    static TimeTable produce(gtfs::Dataset &dataset,
                             std::vector<boost::optional<ShapeID>> &shape_from_line);
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_FACTORY_HPP__
