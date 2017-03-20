#ifndef TRANSIT_SERVICE_TILE_HPP_
#define TRANSIT_SERVICE_TILE_HPP_

#include "search/coordinate_to_stop.hpp"
#include "search/stop_to_line.hpp"
#include "service/interface.hpp"
#include "timetable/timetable.hpp"

#include "algorithm/scc.hpp"
#include "gtfs/stop.hpp"
#include "tool/container/mapbox_vector_tile.hpp"

namespace transit
{
namespace service
{

// The tile service creates a vector tile representation of a network. The parameters specify a
// location x/y/zoom_level in slippy-map tiles. These tile IDs are translated into a graphical
// representation for rendering.
class Tile : public Interface
{
  public:
    Tile(timetable::TimeTable const &timetable, search::CoordinateToStop const &stop_lookup, search::StopToLine const& stop_to_line);

    ServiceStatus operator()(ServiceParameters &parameters) const final override;

  private:
    // create a tile from all stops that are within a bounding box
    tool::container::MapboxVectorTile make_tile(std::vector<gtfs::StopID> const &stops) const;

    // the general network / it's connections
    timetable::TimeTable const &timetable;
    // looking up stations / stops by geometry
    search::CoordinateToStop const &stop_lookup;

    algorithm::StronglyConnectedComponent const components;
};

} // namespace service
} // namespace transit

#endif // TRANSIT_SERVICE_TILE_HPP_
