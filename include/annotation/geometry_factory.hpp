#ifndef NEPOMUK_ANNOTATION_GEOMETRY_FACTORY_HPP_
#define NEPOMUK_ANNOTATION_GEOMETRY_FACTORY_HPP_

#include <vector>

#include "annotation/geometry.hpp"
#include "timetable/segment_table.hpp"
#include "timetable/trip_table.hpp"

namespace nepomuk
{
namespace gtfs
{
struct Stop;
struct Trip;
} // namespace gtfs

namespace timetable
{
class TripTable;
};

struct ShapeID;

namespace annotation
{

class GeometryFactory
{
  public:
    static Geometry produce(timetable::TripTable const &trip_table,
                            std::vector<TripID> const &internal_to_external_trip_mapping,
                            std::vector<gtfs::Stop> const &stops,
                            std::vector<gtfs::Trip> const &trips,
                            timetable::SegmentTable const &segment_table);
};

} // namespace annotation
} // namespace nepomuk

#endif // NEPOMUK_ANNOTATION_GEOMETRY_FACTORY_HPP_
