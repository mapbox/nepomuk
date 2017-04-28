#ifndef TRANSIT_ANNOTATION_OSRM_HPP_
#define TRANSIT_ANNOTATION_OSRM_HPP_

#include "annotation/geometry.hpp"
#include "annotation/stop_info.hpp"
#include "geometric/coordinate.hpp"
#include "navigation/route.hpp"
#include "tool/container/string_table.hpp"

namespace transit
{
namespace annotation
{

// mock implementation of osrm annotation to allow using the osrm frontent for transit queries. The
// results are made to represent the results of an osrm v5.X route.
class OSRM
{
  public:
    OSRM(StopInfoTable const &stop_info,
         transit::tool::container::StringTable const &dictionary,
         Geometry const &geometry);

    // should probably provide a json response at some time, right now we generate an annotated
    // string
    std::string operator()(navigation::Route const &route) const;

  private:
    // to make osrm annotation happy
    void write_intersections(std::ostringstream &oss, StopID const stop) const;
    void write_maneuver(std::ostringstream &oss, StopID const stop, std::string const &type) const;
    void write_segment(std::ostringstream &oss, navigation::segment::Transit const &transit) const;
    void write_segment(std::ostringstream &oss,
                       navigation::segment::Transfer const &transfer) const;
    void write_segment(std::ostringstream &oss, navigation::segment::Walk const &walk) const;
    void write_leg(std::ostringstream &oss, navigation::Leg const &leg) const;
    void write_legs(std::ostringstream &oss, navigation::Route const &route) const;
    void write_route(std::ostringstream &oss, navigation::Route const &leg) const;
    void write_coordinates(std::ostringstream &oss, navigation::Route const &leg) const;
    void write_segments(std::ostringstream &oss, navigation::Leg const &leg) const;
    void write_waypoint(std::ostringstream &oss, StopID const stop) const;
    void write_waypoint(std::ostringstream &oss, geometric::WGS84Coordinate const location) const;
    void write_waypoints(std::ostringstream &oss, navigation::Route const &route) const;

    StopInfoTable const &stop_info;
    transit::tool::container::StringTable const &dictionary;
    Geometry const &geometry;
};

} // namespace annotation
} // namespace transit

#endif // TRANSIT_ANNOTATION_OSRM_HPP_
