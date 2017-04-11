#ifndef TRANSIT_TOOL_CONTAINER_MAPBOX_VECTOR_TILE_HPP_
#define TRANSIT_TOOL_CONTAINER_MAPBOX_VECTOR_TILE_HPP_

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/variant.hpp>
#include <protozero/pbf_writer.hpp>

#include "geometric/bounding_box.hpp"
#include "geometric/coordinate.hpp"

// implementation of https://www.mapbox.com/vector-tiles/specification/

namespace transit
{
namespace tool
{
namespace container
{

enum VectorTileValueType
{
    BOOL,
    INT,
    UINT,
    DOUBLE,
    FLOAT,
    STRING
};

struct VectorTileValue
{
    VectorTileValueType type;
    boost::variant<bool, std::int64_t, std::uint64_t, double, float, std::string> value;

    // comparison so we can use a map
    bool operator==(VectorTileValue const &other) const;
    bool operator<(VectorTileValue const &other) const;

    void write(protozero::pbf_writer &pbf_writer) const;
};

class MapboxVectorTile;

// make sure that the layer goes out of scope to flush the result
class MapboxVectorTileLayer
{
    friend class MapboxVectorTile;

  public:
    // serialises the feature table
    ~MapboxVectorTileLayer();

    void add_point(geometric::WGS84Coordinate const coordinate,
                   std::vector<std::pair<std::string, VectorTileValue>> const &features);

    void add_line(std::vector<geometric::WGS84Coordinate> const &line_string,
                  std::vector<std::pair<std::string, VectorTileValue>> const &features);

  private:
    MapboxVectorTileLayer(protozero::pbf_writer &pbf_writer,
                          std::string const &name,
                          geometric::MercatorBoundingBox bounding_box);

    void encode_features(protozero::pbf_writer &feature_writer,
                         std::vector<std::pair<std::string, VectorTileValue>> const &features);

    void write_key_table();
    void write_value_table();

    std::uint64_t feature_id;
    protozero::pbf_writer layer_writer;

    std::map<std::string, std::size_t> keys;
    std::map<VectorTileValue, std::size_t> values;

    geometric::MercatorBoundingBox bounding_box;
};

// adaptor to create a mapbox vector tile from the transit data
class MapboxVectorTile
{
  public:
    using FeatureList = std::vector<std::pair<std::string, VectorTileValue>>;

    MapboxVectorTile(std::uint32_t const horizontal = 0,
                     std::uint32_t const vertical = 0,
                     std::uint32_t const zoom_level = 0);

    // allow easy assignment in json creation
    operator std::string() const;

    // request a new layer, only keep a single per vector tile around.
    MapboxVectorTileLayer new_layer(std::string const &name);

  private:
    std::string pbf_buffer;
    protozero::pbf_writer tile_writer;

    geometric::MercatorBoundingBox bounding_box;
};

} // namespace container
} // namespace tool
} // namespace transit

#endif // TRANSIT_TOOL_CONTAINER_MAPBOX_VECTOR_TILE_HPP_
