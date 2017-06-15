#include "service/tile.hpp"
#include "timetable/graph_adaptor.hpp"

#include <algorithm>
#include <iomanip>
#include <iterator>
#include <set>
#include <tuple>

#include <boost/assert.hpp>

namespace nepomuk
{
namespace service
{

Tile::Tile(service::Master &master_service)
    : timetable(master_service.timetable()), stop_lookup(master_service.coordinate_to_stop()),
      stop_to_line(master_service.stop_to_line()), dictionary(master_service.dictionary()),
      stop_annotation(master_service.stop_annotation()),
      geometry(master_service.geometry_annotation()), segment_table(master_service.segment_table()),
      components(master_service.components())
{
}

tool::container::MapboxVectorTile Tile::operator()(TileParameters &parameters) const
{
    BOOST_ASSERT(parameters.valid());

    geometric::WGS84BoundingBox const bounding_box(
        parameters.horizontal_id(), parameters.vertical_id(), parameters.zoom_level(), 256.0, 1024);
    return make_tile(parameters.horizontal_id(),
                     parameters.vertical_id(),
                     parameters.zoom_level(),
                     stop_lookup.all(bounding_box));
}

tool::container::MapboxVectorTile Tile::make_tile(std::uint32_t const horizontal,
                                                  std::uint32_t const vertical,
                                                  std::uint32_t const zoom_level,
                                                  StopsFromLookup const &stops) const
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
                     StopsFromLookup const &stops) const
{

    auto connection_layer = vector_tile.new_layer("lines");

    std::set<std::tuple<StopID, StopID, LineID>> added_shapes;

    for (auto const pair : stops)
    {
        auto const stop = pair.first;
        auto const lines = stop_to_line(stop);
        for (auto const line_id : lines)
        {
            auto const stop_range = timetable.line(line_id).stops().list(stop);
            if (std::distance(stop_range.begin(), stop_range.end()) > 1)
            {
                // find the appropriate line id in both entries stop_info_annotation:
                auto const from_stop = *stop_range.begin();
                auto const to_stop = *(stop_range.begin() + 1);

                // ask for an inclusive range (to is an actual valid value, not the end)
                auto range = geometry.get(line_id, from_stop, to_stop);
                if (!range.empty())
                {
                    auto tuple = std::make_tuple(from_stop, to_stop, line_id);
                    if (!added_shapes.count(tuple))
                    {
                        std::vector<geometric::WGS84Coordinate> line(range.begin(), range.end());
                        connection_layer.add_line(line, {});
                        added_shapes.insert(tuple);
                    }
                }
                else
                {
                    auto const tuple =
                        std::make_tuple(from_stop, to_stop, LineID{static_cast<std::uint64_t>(-1)});
                    if (added_shapes.count(tuple))
                        continue;
                    std::vector<geometric::WGS84Coordinate> line;
                    line.push_back(geometry.get(from_stop));
                    line.push_back(geometry.get(to_stop));
                    connection_layer.add_line(line, {});
                    added_shapes.insert(tuple);
                }
            }
        }
    }
}

void Tile::add_stops(tool::container::MapboxVectorTile &vector_tile,
                     StopsFromLookup const &stops) const
{
    auto station_layer = vector_tile.new_layer("stations");

    std::vector<StopID> stations;
    stations.reserve(stops.size());
    std::transform(stops.begin(),
                   stops.end(),
                   std::back_inserter(stations),
                   [this](auto const stop) { return timetable.station(stop.first); });
    std::sort(stations.begin(), stations.end());
    stations.erase(std::unique(stations.begin(), stations.end()), stations.end());

    auto const add_station = [this, &station_layer](auto const station) {
        auto const info = stop_annotation(station);
        tool::container::MapboxVectorTile::FeatureList features;
        features.push_back(
            {"name",
             {tool::container::VectorTileValueType::STRING, dictionary.get_string(info.name_id)}});
        station_layer.add_point(geometry.get(station), features);
    };

    std::for_each(stations.begin(), stations.end(), add_station);
}

void Tile::add_transfers(tool::container::MapboxVectorTile &vector_tile,
                         StopsFromLookup const &stops) const
{
    auto transfer_layer = vector_tile.new_layer("transfers");
    for (auto const stop : stops)
    {
        auto const transfers = timetable.transfers(stop.first);
        for (auto transfer : transfers)
        {
            if (transfer.stop_id != stop.first)
            {
                std::vector<geometric::WGS84Coordinate> line;
                line.push_back(geometry.get(stop.first));
                line.push_back(geometry.get(transfer.stop_id));
                transfer_layer.add_line(line, {});
            }
        }
    }
}

void Tile::add_components(tool::container::MapboxVectorTile &vector_tile,
                          StopsFromLookup const &stops) const
{
    auto component_layer = vector_tile.new_layer("components");
    tool::container::MapboxVectorTile::FeatureList from_features;
    tool::container::MapboxVectorTile::FeatureList to_features;
    from_features.push_back({"origin", {tool::container::VectorTileValueType::BOOL, true}});
    to_features.push_back({"destinantion", {tool::container::VectorTileValueType::BOOL, true}});
    for (auto const pair : stops)
    {
        auto const stop = pair.first;
        auto const lines = stop_to_line(stop);
        for (auto const line_id : lines)
        {
            auto const stop_range = timetable.line(line_id).stops().list(stop);
            if (std::distance(stop_range.begin(), stop_range.end()) > 1)
            {
                auto const next_stop = *(stop_range.begin() + 1);
                if (components.component(stop.base()) != components.component(next_stop.base()))
                {
                    component_layer.add_point(geometry.get(stop), from_features);
                    component_layer.add_point(geometry.get(next_stop), to_features);
                }
            }
        }
        if (components.size(components.component(stop.base())) == 1)
        {
            component_layer.add_point(geometry.get(stop), from_features);
        }
    }
}

} // namespace service
} // namespace nepomuk
