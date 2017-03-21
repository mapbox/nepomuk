#include "service/tile.hpp"
#include "timetable/graph_adaptor.hpp"

#include <algorithm>
#include <iterator>

namespace transit
{
namespace service
{

Tile::Tile(timetable::TimeTable const &timetable,
           search::CoordinateToStop const &stop_lookup,
           search::StopToLine const &stop_to_line,
           transit::tool::container::StringTable const &dictionary,
           annotation::StopInfoTable const &stop_info_annotation)
    : timetable(timetable), stop_lookup(stop_lookup), stop_to_line(stop_to_line),
      dictionary(dictionary), stop_info_annotation(stop_info_annotation),
      components(
          algorithm::computeSCC(timetable::TimetableToGraphAdaptor::adapt(timetable, stop_to_line)))
{
}

ServiceStatus Tile::operator()(ServiceParameters &parameters) const
{
    auto &tile_parameters = boost::get<TileParameters>(parameters);

    if (!tile_parameters.valid())
        return ServiceStatus::ERROR;

    geometric::WGS84BoundingBox bounding_box(tile_parameters.horizontal_id(),
                                             tile_parameters.vertical_id(),
                                             tile_parameters.zoom_level());
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
    std::vector<gtfs::StopID> stations;
    stations.reserve(stops.size());
    std::transform(stops.begin(),
                   stops.end(),
                   std::back_inserter(stations),
                   [this](auto const stop) { return timetable.station(stop); });
    std::sort(stations.begin(), stations.end());
    stations.erase(std::unique(stations.begin(), stations.end()), stations.end());

    // compute a vector tile from a set of stops
    tool::container::MapboxVectorTile vector_tile(horizontal, vertical, zoom_level);

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

    {
        auto station_layer = vector_tile.new_layer("stations");

        auto const add_station = [this, &station_layer](auto const station) {
            auto const info = stop_info_annotation.get_info(station);
            tool::container::MapboxVectorTile::FeatureList features;
            features.push_back({"name",
                                {tool::container::VectorTileValueType::STRING,
                                 dictionary.get_string(info.name_id)}});
            station_layer.add_point(info.location, features);
        };

        std::for_each(stations.begin(), stations.end(), add_station);
    }

    return vector_tile;
}

} // namespace service
} // namespace transit
