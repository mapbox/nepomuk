#include "gtfs/shape.hpp"
#include "gtfs/constructor.hpp"

namespace transit
{
namespace gtfs
{

bool checkShapeCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("shape_id") && header.count("shape_pt_lat") &&
           header.count("shape_pt_lon") && header.count("shape_pt_sequence");
}

Shape makeShape(std::map<std::string, std::size_t> const &header, std::vector<std::string> &values)
{
    const auto dist_traveled_or_empty =
        construct<std::string>("shape_dist_traveled", forward, header, values);
    boost::optional<double> dist_traveled = boost::none;
    if (!dist_traveled_or_empty.empty())
        dist_traveled = std::stof(dist_traveled_or_empty);

    const double lat = std::stof(construct<std::string>("shape_pt_lat", forward, header, values));
    const double lon = std::stof(construct<std::string>("shape_pt_lon", forward, header, values));

    return {
        construct<ShapeID>("shape_id", stringToID<ShapeID>, header, values),
        geometric::WGS84Coordinate(geometric::makeLatLonFromDouble<geometric::FixedLongitude>(lon),
                                   geometric::makeLatLonFromDouble<geometric::FixedLatitude>(lat)),
        construct<SequenceID>("shape_pt_sequence", stringToID<SequenceID>, header, values),
        dist_traveled};
}

} // namespace gtfs
} // namespace transit
