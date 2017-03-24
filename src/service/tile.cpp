#include "service/tile.hpp"
#include "timetable/graph_adaptor.hpp"

#include <algorithm>
#include <iterator>

#include <fstream>

namespace transit
{
namespace service
{

Tile::Tile(service::Master &master_service)
    : timetable(master_service.timetable()), stop_lookup(master_service.coordinate_to_stop()),
      stop_to_line(master_service.stop_to_line()), dictionary(master_service.dictionary()),
      stop_info_annotation(master_service.stop_info_annotation()),
      components(master_service.components())
{
}

ServiceStatus Tile::operator()(ServiceParameters &parameters) const
{
    auto &tile_parameters = boost::get<TileParameters>(parameters);

    if (!tile_parameters.valid())
        return ServiceStatus::ERROR;

    geometric::WGS84BoundingBox const bounding_box(tile_parameters.horizontal_id(),
                                                   tile_parameters.vertical_id(),
                                                   tile_parameters.zoom_level(),
                                                   256.0,
                                                   1024);
    tile_parameters._result = make_tile(tile_parameters.horizontal_id(),
                                        tile_parameters.vertical_id(),
                                        tile_parameters.zoom_level(),
                                        stop_lookup.all(bounding_box));

    return ServiceStatus::SUCCESS;
}

tool::container::MapboxVectorTile Tile::make_tile(std::uint32_t const horizontal,
                                                  std::uint32_t const vertical,
                                                  std::uint32_t const zoom_level,
                                                  std::vector<gtfs::StopID> const &stops) const
{
    // compute a vector tile from a set of stops
    tool::container::MapboxVectorTile vector_tile(horizontal, vertical, zoom_level);

    add_lines(vector_tile, stops);
    add_transfers(vector_tile, stops);
    add_components(vector_tile, stops);
    add_stops(vector_tile, stops);

    return vector_tile;
}

void Tile::add_lines(tool::container::MapboxVectorTile &vector_tile,
                     std::vector<gtfs::StopID> const &stops) const
{

    auto connection_layer = vector_tile.new_layer("lines");
    for (auto const stop : stops)
    {
        auto const lines = stop_to_line(stop);
        for (auto const line_id : lines)
        {
            auto const trip = timetable.list_trips(line_id, gtfs::Time("00:00:00"));
            if (trip)
            {
                auto const stop_range = trip->stop_table.list(stop);
                if (std::distance(stop_range.begin(), stop_range.end()) > 1)
                {
                    std::vector<geometric::WGS84Coordinate> line;
                    line.push_back(stop_info_annotation.get_info(*stop_range.begin()).location);
                    line.push_back(
                        stop_info_annotation.get_info(*(stop_range.begin() + 1)).location);
                    connection_layer.add_line(line, {});
                }
            }
        }
    }
}

void Tile::add_stops(tool::container::MapboxVectorTile &vector_tile,
                     std::vector<gtfs::StopID> const &stops) const
{
    auto station_layer = vector_tile.new_layer("stations");

    std::vector<gtfs::StopID> stations;
    stations.reserve(stops.size());
    std::transform(stops.begin(),
                   stops.end(),
                   std::back_inserter(stations),
                   [this](auto const stop) { return timetable.station(stop); });
    std::sort(stations.begin(), stations.end());
    stations.erase(std::unique(stations.begin(), stations.end()), stations.end());

    auto const add_station = [this, &station_layer](auto const station) {
        auto const info = stop_info_annotation.get_info(station);
        tool::container::MapboxVectorTile::FeatureList features;
        features.push_back(
            {"name",
             {tool::container::VectorTileValueType::STRING, dictionary.get_string(info.name_id)}});
        station_layer.add_point(info.location, features);
    };

    std::for_each(stations.begin(), stations.end(), add_station);
}

void Tile::add_transfers(tool::container::MapboxVectorTile &vector_tile,
                         std::vector<gtfs::StopID> const &stops) const
{
    auto transfer_layer = vector_tile.new_layer("transfers");
    for (auto const stop : stops)
    {
        auto const transfers = timetable.list_transfers(stop);
        for (auto transfer : transfers)
        {
            if (transfer.stop_id != stop)
            {
                std::vector<geometric::WGS84Coordinate> line;
                line.push_back(stop_info_annotation.get_info(stop).location);
                line.push_back(stop_info_annotation.get_info(transfer.stop_id).location);
                transfer_layer.add_line(line, {});
            }
        }
    }
}

void Tile::add_components(tool::container::MapboxVectorTile &vector_tile,
                          std::vector<gtfs::StopID> const &stops) const
{
    auto component_layer = vector_tile.new_layer("components");
    tool::container::MapboxVectorTile::FeatureList from_features;
    tool::container::MapboxVectorTile::FeatureList to_features;
    from_features.push_back({"origin", {tool::container::VectorTileValueType::BOOL, true}});
    to_features.push_back({"destinantion", {tool::container::VectorTileValueType::BOOL, true}});
    for (auto const stop : stops)
    {
        auto const lines = stop_to_line(stop);
        for (auto const line_id : lines)
        {
            auto const trip = timetable.list_trips(line_id, gtfs::Time("00:00:00"));
            if (trip)
            {
                auto const stop_range = trip->stop_table.list(stop);
                if (std::distance(stop_range.begin(), stop_range.end()) > 1)
                {
                    auto const next_stop = *(stop_range.begin() + 1);
                    if (components.component(stop.base()) != components.component(next_stop.base()))
                    {
                        component_layer.add_point(stop_info_annotation.get_info(stop).location,
                                                  from_features);
                        component_layer.add_point(stop_info_annotation.get_info(next_stop).location,
                                                  to_features);
                    }
                }
            }
        }
    }
}

} // namespace service
} // namespace transit
