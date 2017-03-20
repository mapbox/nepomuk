#include "service/tile.hpp"
#include "timetable/graph_adaptor.hpp"

namespace transit
{
namespace service
{

Tile::Tile(timetable::TimeTable const &timetable,
           search::CoordinateToStop const &stop_lookup,
           search::StopToLine const &stop_to_line)
    : timetable(timetable), stop_lookup(stop_lookup),
      components(
          algorithm::computeSCC(timetable::TimetableToGraphAdaptor::adapt(timetable, stop_to_line)))
{
}

ServiceStatus Tile::operator()(ServiceParameters &parameters) const
{
    auto &tile_parameters = boost::get<TileParameters>(parameters);

    if (!tile_parameters.valid())
        return ServiceStatus::ERROR;

    geometric::Coordinate lower_left, upper_right;
    geometric::BoundingBox bounding_box{lower_left, upper_right};
    tile_parameters._result = make_tile(stop_lookup.all(bounding_box));

    return ServiceStatus::SUCCESS;
}

tool::container::MapboxVectorTile Tile::make_tile(std::vector<gtfs::StopID> const &stops) const
{
    // compute a vector tile from a set of stops
    return {};
}

} // namespace service
} // namespace transit
