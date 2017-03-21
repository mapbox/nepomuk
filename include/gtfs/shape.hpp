#ifndef TRANSIT_GTFS_SHAPE_HPP_
#define TRANSIT_GTFS_SHAPE_HPP_

#include <boost/optional.hpp>

#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "geometric/coordinate.hpp"
#include "gtfs/sequence.hpp"
#include "tool/types_and_traits/strong_typedef.hpp"

namespace transit
{
namespace gtfs
{

STRONG_ID_TYPE(std::uint64_t, ShapeID)

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
} // namespace transit

// needs to be outside of namespaces
STRONG_ID_TYPE_HASH(std::uint64_t, transit::gtfs::ShapeID)
#endif // TRANSIT_GTFS_SHAPE_HPP_
