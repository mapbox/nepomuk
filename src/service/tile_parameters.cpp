#include "service/tile_parameters.hpp"

#include <cmath>

namespace transit
{
namespace service
{

TileParameters::TileParameters(std::uint32_t x, std::uint32_t y, std::uint32_t zoom)
: _horizontal_id(x), _vertical_id(y), _zoom_level(zoom)
{}

std::uint32_t TileParameters::horizontal_id() const { return _horizontal_id; }
std::uint32_t TileParameters::vertical_id() const { return _vertical_id; }
std::uint32_t TileParameters::zoom_level() const { return _zoom_level; }

bool TileParameters::valid() const
{
    // https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#Zoom_levels
    // https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#X_and_Y
    const auto valid_horizontal =
        _horizontal_id <= static_cast<unsigned>(std::pow(2., _zoom_level)) - 1;
    const auto valid_vertical =
        _vertical_id <= static_cast<unsigned>(std::pow(2., _zoom_level)) - 1;
    // zoom limits are due to slippy map and server performance limits
    const auto valid_zoom = _zoom_level < 20 && _zoom_level >= 12;

    return valid_horizontal && valid_vertical && valid_zoom;
}

} // namespace service
} // namespace transit
