#ifndef TRANSIT_ANNOTATION_API_HPP_
#define TRANSIT_ANNOTATION_API_HPP_

#include "annotation/geometry.hpp"
#include "geometric/coordinate.hpp"
#include "navigation/route.hpp"
#include <iostream>
#include <string>

namespace transit
{

// forward declaration
namespace navigation
{
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

namespace annotation
{
class API
{
  public:
    API(Geometry const &geometry);
    std::string operator()(std::vector<navigation::Route> const &route) const;

  private:
    void jsonify_waypoints(std::ostream &os, navigation::Route const &route) const;
    void jsonify(std::ostream &os, navigation::Route const &route) const;
    void jsonify(std::ostream &os, navigation::Leg const &leg) const;
    void jsonify(std::ostream &os, navigation::Segment const &segment) const;
    void jsonify(std::ostream &os, navigation::segment::Transit const &transit) const;
    void jsonify(std::ostream &os, navigation::segment::Transfer const &transfer) const;
    void jsonify(std::ostream &os, navigation::segment::Walk const &walk) const;
    void jsonify(std::ostream &os, navigation::Stop const &stop) const;
    void jsonify(std::ostream &os, geometric::WGS84Coordinate const coordinate) const;
    void jsonify(std::ostream &os,
                 std::vector<geometric::WGS84Coordinate> const &coordinates) const;
    void jsonify(std::ostream &os, navigation::Connection const &connection) const;

    // write an array of elements for which a jsonify function exists
    template <typename object_type_itr>
    void chain_jsonify(std::ostream &os, object_type_itr begin, object_type_itr const end) const;

    Geometry const &geometry;
};

template <typename object_type_itr>
void API::chain_jsonify(std::ostream &os, object_type_itr begin, object_type_itr const end) const
{
    BOOST_ASSERT(begin != end);
    os << "[";
    jsonify(os, *begin++);
    for (; begin != end; ++begin)
        os << ",", jsonify(os, *begin);
    os << "]";
}

} // namespace annotation
} // namspace transit

#endif // TRANSIT_ANNOTATION_API_HPP_
