#ifndef NEPOMUK_ANNOTATION_PBF_HPP_
#define NEPOMUK_ANNOTATION_PBF_HPP_

#include <boost/assert.hpp> // for BOOST_ASSERT
#include <iostream>
#include <vector> // for vector

#include "ipc/route_response.pb.h"
#include "tool/container/mapbox_vector_tile.hpp"

namespace nepomuk
{

// forward declaration
namespace geometric
{
class WGS84Coordinate;
} // namespace geometric;

namespace navigation
{
class Route;
class Leg;
class Segment;
class Connection;
class Stop;

namespace segment
{
class Transit;
class Transfer;
class Walk;
} // namespace segment
} // namespace navigation

namespace tool
{
namespace container
{
class StringTable;
} // namespace container;
} // namespace tool

namespace annotation
{

class Geometry;
class Stop;
class Line;

class PBF
{
  public:
    PBF(tool::container::StringTable const &string_table,
        Geometry const &geometry,
        Stop const &stop,
        Line const &line);
    void operator()(ipc::RouteResponse &result, std::vector<navigation::Route> const &route) const;

  private:
    void add_waypoints(ipc::RouteResponse &target, navigation::Route const &route) const;
    void add(ipc::Route &target, navigation::Route const &route) const;
    void add(ipc::Leg &target, navigation::Leg const &leg) const;
    void add(ipc::Segment &target, navigation::Segment const &segment) const;
    void add(ipc::TransitSegment &target, navigation::segment::Transit const &transit) const;
    void add(ipc::TransferSegment &target, navigation::segment::Transfer const &transfer) const;
    void add(ipc::WalkingSegment &target, navigation::segment::Walk const &walk) const;
    void add(ipc::Stop &target, navigation::Stop const &stop) const;
    void add(ipc::Coordinate &target, geometric::WGS84Coordinate const coordinate) const;
    void add(ipc::Connection &target, navigation::Connection const &connection) const;

    // Lookup to translate ids into strings
    tool::container::StringTable const &string_table;

    // sub-annotations to be used during the response generation
    Geometry const &geometry;
    Stop const &stop_annotation;
    Line const &line_annotation;
};

} // namespace annotation
} // namespace transit

#endif // NEPOMUK_ANNOTATION_PBF_HPP_
