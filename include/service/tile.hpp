#ifndef NEPOMUK_SERVICE_TILE_HPP_
#define NEPOMUK_SERVICE_TILE_HPP_

#include "annotation/stop_info.hpp"
#include "search/coordinate_to_stop.hpp"
#include "search/stop_to_line.hpp"
#include "service/interface.hpp"
#include "service/master.hpp"
#include "timetable/segment_table.hpp"
#include "timetable/timetable.hpp"

#include "adaptor/dictionary.hpp"
#include "algorithm/scc.hpp"
#include "id/stop.hpp"
#include "tool/container/mapbox_vector_tile.hpp"

#include <cstdint>

namespace nepomuk
{
namespace service
{

// The tile service creates a vector tile representation of a network. The parameters specify a
// location x/y/zoom_level in slippy-map tiles. These tile IDs are translated into a graphical
// representation for rendering.
class Tile : public Interface
{
  public:
    Tile(service::Master &master_service);
    ~Tile() final override = default;

    ServiceStatus operator()(ServiceParameters &parameters) const final override;

  private:
    using StopsFromLookup = std::vector<search::CoordinateToStop::value_type>;
    // create a tile from all stops that are within a bounding box
    tool::container::MapboxVectorTile make_tile(std::uint32_t const horizontal,
                                                std::uint32_t const vertical,
                                                std::uint32_t const zoom_level,
                                                StopsFromLookup const &stops) const;

    // the general network / it's connections
    timetable::TimeTable const &timetable;
    // looking up stations / stops by geometry
    search::CoordinateToStop const &stop_lookup;
    search::StopToLine const &stop_to_line;
    // to request coordinates / names
    nepomuk::tool::container::StringTable const &dictionary;
    annotation::StopInfoTable const &stop_info_annotation;
    // the lines and their actual geometry
    annotation::Geometry const &geometry;
    timetable::SegmentTable const &segment_table;

    algorithm::StronglyConnectedComponent const components;

    void add_lines(tool::container::MapboxVectorTile &vector_tile,
                   StopsFromLookup const &stops) const;
    void add_stops(tool::container::MapboxVectorTile &vector_tile,
                   StopsFromLookup const &stops) const;
    void add_transfers(tool::container::MapboxVectorTile &vector_tile,
                       StopsFromLookup const &stops) const;
    void add_components(tool::container::MapboxVectorTile &vector_tile,
                        StopsFromLookup const &stops) const;
};

} // namespace service
} // namespace nepomuk

#endif // NEPOMUK_SERVICE_TILE_HPP_
