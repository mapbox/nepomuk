#ifndef TRANSIT_TIMETABLE_FACTORY_HPP_
#define TRANSIT_TIMETABLE_FACTORY_HPP_

#include <boost/optional.hpp>
#include <vector>

#include "timetable/timetable.hpp"

namespace transit
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
} // namespace transit

#endif // TRANSIT_TIMETABLE_FACTORY_HPP__
