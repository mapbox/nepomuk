#ifndef TRANSIT_ANNOTATION_TRIP_HPP_
#define TRANSIT_ANNOTATION_TRIP_HPP_

#include "annotation/geometry.hpp"
#include "annotation/stop_info.hpp"
#include "navigation/trip.hpp"
#include "tool/container/string_table.hpp"

namespace transit
{
namespace annotation
{

class Trip
{
  public:
    Trip(StopInfoTable const &stop_info,
         Geometry const &geometry,
         transit::tool::container::StringTable const &dictionary);

    // should probably provide a json response at some time, right now we generate an annotated
    // string
    std::string operator()(navigation::Trip const &trip) const;

  private:
    StopInfoTable const &stop_info;
    Geometry const &geometry;
    transit::tool::container::StringTable const &dictionary;
};

} // namespace annotation
} // namespace transit

#endif // TRANSIT_ANNOTATION_TRIP_HPP_
