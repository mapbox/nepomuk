#ifndef TRANSIT_SERVICE_TILE_PARAMETERS_HPP_
#define TRANSIT_SERVICE_TILE_PARAMETERS_HPP_

#include <cstdint>

#include "tool/container/mapbox_vector_tile.hpp"

namespace transit
{
namespace service
{

class TileParameters final
{
  public:
    TileParameters(std::uint32_t x = 0, std::uint32_t y = 0, std::uint32_t zoom = 0);
    bool valid() const;

    std::uint32_t horizontal_id() const;
    std::uint32_t vertical_id() const;
    std::uint32_t zoom_level() const;

    auto const &result() const { return _result; }
    auto &result() { return _result; }

  private:
    std::uint32_t _horizontal_id;
    std::uint32_t _vertical_id;
    std::uint32_t _zoom_level;

    tool::container::MapboxVectorTile _result;

    friend class Tile;
};

} // namespace service
} // namespace transit

#endif // TRANSIT_SERVICE_TILE_PARAMETERS_HPP_
