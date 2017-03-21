#ifndef TRANSIT_SERVICE_TILE_HPP_
#define TRANSIT_SERVICE_TILE_HPP_

#include "annotation/stop_info.hpp"
#include "search/coordinate_to_stop.hpp"
#include "search/stop_to_line.hpp"
#include "service/interface.hpp"
#include "timetable/timetable.hpp"

#include "adaptor/dictionary.hpp"
#include "algorithm/scc.hpp"
#include "gtfs/stop.hpp"
#include "tool/container/mapbox_vector_tile.hpp"

#include <cstdint>

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
    Tile(timetable::TimeTable const &timetable,
         search::CoordinateToStop const &stop_lookup,
         search::StopToLine const &stop_to_line,
         transit::tool::container::StringTable const &dictionary,
         annotation::StopInfoTable const &stop_info_annotation);

    ServiceStatus operator()(ServiceParameters &parameters) const final override;

  private:
    // create a tile from all stops that are within a bounding box
    tool::container::MapboxVectorTile make_tile(std::uint32_t const horizontal,
                                                std::uint32_t const vertical,
                                                std::uint32_t const zoom_level,
                                                std::vector<gtfs::StopID> const &stops) const;

    // the general network / it's connections
    timetable::TimeTable const &timetable;
    // looking up stations / stops by geometry
    search::CoordinateToStop const &stop_lookup;
    search::StopToLine const &stop_to_line;
    // to request coordinates / names
    transit::tool::container::StringTable const &dictionary;
    annotation::StopInfoTable const &stop_info_annotation;

    algorithm::StronglyConnectedComponent const components;
};

} // namespace service
} // namespace transit

#endif // TRANSIT_SERVICE_TILE_HPP_
