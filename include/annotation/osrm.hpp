#ifndef TRANSIT_ANNOTATION_OSRM_HPP_
#define TRANSIT_ANNOTATION_OSRM_HPP_

#include "annotation/geometry.hpp"
#include "annotation/stop_info.hpp"
#include "navigation/trip.hpp"
#include "tool/container/string_table.hpp"

namespace transit
{
namespace annotation
{

// mock implementation of osrm annotation to allow using the osrm frontent for transit queries. The
// results are made to represent the results of an osrm v5.X trip.
class OSRM
{
  public:
    OSRM(StopInfoTable const &stop_info,
         transit::tool::container::StringTable const &dictionary,
         Geometry const &geometry);

    // should probably provide a json response at some time, right now we generate an annotated
    // string
    std::string operator()(navigation::Trip const &trip) const;

  private:
    void write_maneuver(std::ostringstream &oss, StopID const stop, std::string const &type) const;
    void write_leg(std::ostringstream &oss, navigation::Leg const &leg) const;
    void write_legs(std::ostringstream &oss, navigation::Trip const &trip) const;
    void write_trip(std::ostringstream &oss, navigation::Trip const &leg) const;
    void write_coordinates(std::ostringstream &oss, navigation::Trip const &leg) const;
    void write_steps(std::ostringstream &oss, navigation::Leg const &leg) const;

    StopInfoTable const &stop_info;
    transit::tool::container::StringTable const &dictionary;
    Geometry const &geometry;
};

} // namespace annotation
} // namespace transit

#endif // TRANSIT_ANNOTATION_OSRM_HPP_
