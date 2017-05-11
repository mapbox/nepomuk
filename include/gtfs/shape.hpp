#ifndef NEPOMUK_GTFS_SHAPE_HPP_
#define NEPOMUK_GTFS_SHAPE_HPP_

#include <boost/optional.hpp>

#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "geometric/coordinate.hpp"
#include "id/sequence.hpp"
#include "id/shape.hpp"

namespace nepomuk
{
namespace gtfs
{

struct Shape
{
    ShapeID id;
    geometric::WGS84Coordinate location;
    SequenceID sequence_id;

    boost::optional<double> distance_traveled;
};

bool checkShapeCSVHeader(std::map<std::string, std::size_t> const &header);
Shape makeShape(std::map<std::string, std::size_t> const &header, std::vector<std::string> &values);

} // namespace gtfs
} // namespace nepomuk

#endif // NEPOMUK_GTFS_SHAPE_HPP_
