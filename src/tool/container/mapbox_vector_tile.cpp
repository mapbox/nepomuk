#include "tool/container/mapbox_vector_tile.hpp"

#include <protozero/pbf_writer.hpp>
#include <protozero/varint.hpp>

#include <boost/assert.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/multi/geometries/multi_linestring.hpp>
#pragma GCC diagnostic pop

#include <functional>

namespace transit
{
namespace tool
{
namespace container
{

namespace vector_tile
{
const constexpr std::uint32_t ID_TAG = 1;
const constexpr std::uint32_t NAME_TAG = 1;
const constexpr std::uint32_t FEATURE_TAG = 2;
const constexpr std::uint32_t LAYER_TAG = 3;
const constexpr std::uint32_t GEOMETRY_TAG = 3;
const constexpr std::uint32_t KEY_TAG = 3;
const constexpr std::uint32_t VARIANT_TAG = 4;
const constexpr std::uint32_t EXTENT_TAG = 5;
const constexpr std::uint32_t VERSION_TAG = 15;

const constexpr std::uint32_t FEATURE_ATTRIBUTES_TAG = 2;
const constexpr std::uint32_t FEATURE_GEOMETRIES_TAG = 4;

const constexpr std::uint32_t GEOMETRY_TYPE_POINT = 1;
const constexpr std::uint32_t GEOMETRY_TYPE_LINE = 2;

const constexpr std::uint32_t VARIANT_TYPE_STRING = 1;
const constexpr std::uint32_t VARIANT_TYPE_FLOAT = 2;
const constexpr std::uint32_t VARIANT_TYPE_DOUBLE = 3;

const constexpr std::uint32_t VARIANT_TYPE_UINT64 = 5;
const constexpr std::uint32_t VARIANT_TYPE_SINT64 = 6;
const constexpr std::uint32_t VARIANT_TYPE_BOOL = 7;

// Vector tiles are 4096 virtual pixels on each side
const constexpr double EXTENT = 4096.0;
// half the size of a vector tile (pixels != size)
const constexpr double BUFFER = 128.0;
const constexpr double TILE_SIZE = 2 * 128.0;

const constexpr int MOVETO_COMMAND = 9;
} // namespace vector_tile

namespace
{
template <typename content_type>
std::size_t get_map_index(std::map<content_type, std::size_t> &map, content_type const &content)
{
    auto const itr = map.find(content);
    if (itr == map.end())
    {
        auto result = map.size();
        map.insert(std::make_pair(content, result));
        return result;
    }
    else
        return itr->second;
}

using IPoint = boost::geometry::model::point<std::int64_t, 2, boost::geometry::cs::cartesian>;
using Linestring = boost::geometry::model::linestring<IPoint>;
using MultiLinestring = boost::geometry::model::multi_linestring<Linestring>;
using ClipBox = boost::geometry::model::box<IPoint>;

// clipping a single tile
const static ClipBox clip_box(IPoint(-vector_tile::BUFFER, -vector_tile::BUFFER),
                              IPoint(vector_tile::EXTENT + vector_tile::BUFFER,
                                     vector_tile::EXTENT + vector_tile::BUFFER));

IPoint to_tile_point(geometric::WGS84Coordinate coordiante,
                     geometric::MercatorBoundingBox const &bounding_box)
{
    geometric::MercatorCoordinate merc(std::move(coordiante));

    auto const px_longitude = (geometric::doubleFromLatLon(merc.longitude) -
                               geometric::doubleFromLatLon(bounding_box.lower_left.longitude)) /
                              bounding_box.width();
    auto const px_latitude = (geometric::doubleFromLatLon(merc.latitude) -
                              geometric::doubleFromLatLon(bounding_box.lower_left.latitude)) /
                             bounding_box.height();

    return {std::round(px_longitude * vector_tile::EXTENT),
            std::round(px_latitude * vector_tile::EXTENT)};
}

} // namespace

bool VectorTileValue::operator==(VectorTileValue const &other) const
{
    // on equal types, we need to check the contained types, otherwise both entries are not empty
    if (type == other.type)
    {
        switch (type)
        {
        case VectorTileValueType::BOOL:
            return boost::get<bool>(value) == boost::get<bool>(other.value);
        case VectorTileValueType::INT:
            return boost::get<int>(value) == boost::get<int>(other.value);
        case VectorTileValueType::DOUBLE:
            return boost::get<double>(value) == boost::get<double>(other.value);
        default:
            BOOST_ASSERT(type == VectorTileValueType::STRING);
            return boost::get<std::string>(value) == boost::get<std::string>(other.value);
        }
    }
    else
        return false;
}

bool VectorTileValue::operator<(VectorTileValue const &other) const
{
    if (type == other.type)
    {
        switch (type)
        {
        case VectorTileValueType::BOOL:
            return boost::get<bool>(value) < boost::get<bool>(other.value);
        case VectorTileValueType::INT:
            return boost::get<int>(value) < boost::get<int>(other.value);
        case VectorTileValueType::DOUBLE:
            return boost::get<double>(value) < boost::get<double>(other.value);
        default:
            BOOST_ASSERT(type == VectorTileValueType::STRING);
            return boost::get<std::string>(value) < boost::get<std::string>(other.value);
        }
    }
    else
    {
        return type < other.type;
    }
}

void VectorTileValue::write(protozero::pbf_writer &pbf_writer) const
{
    protozero::pbf_writer values_writer(pbf_writer, vector_tile::VARIANT_TAG);

    switch (type)
    {
    case VectorTileValueType::BOOL:
        values_writer.add_bool(vector_tile::VARIANT_TYPE_BOOL, boost::get<bool>(value));
        break;
    case VectorTileValueType::INT:
        values_writer.add_uint64(vector_tile::VARIANT_TYPE_UINT64, boost::get<int>(value));
        break;
    case VectorTileValueType::DOUBLE:
        values_writer.add_double(vector_tile::VARIANT_TYPE_DOUBLE, boost::get<double>(value));
        break;
    default:
        BOOST_ASSERT(type == VectorTileValueType::STRING);
        values_writer.add_string(vector_tile::VARIANT_TYPE_STRING, boost::get<std::string>(value));
        break;
    }
}

MapboxVectorTileLayer::MapboxVectorTileLayer(protozero::pbf_writer &pbf_writer,
                                             std::string const &name,
                                             geometric::MercatorBoundingBox bounding_box)
    : feature_id(1), layer_writer(pbf_writer, vector_tile::LAYER_TAG),
      bounding_box(std::move(bounding_box))
{
    // each layer starts of with its version
    layer_writer.add_uint32(vector_tile::VERSION_TAG, 2);
    // Field 1 is the "layer name" field, it's a string
    layer_writer.add_string(vector_tile::NAME_TAG, name);
    // Field 5 is the tile extent. It's a uint32 and should be set to 4096
    layer_writer.add_uint32(vector_tile::EXTENT_TAG, vector_tile::EXTENT);
}

MapboxVectorTileLayer::~MapboxVectorTileLayer()
{
    // write the key/value tables
    write_key_table();
    write_value_table();
}

void MapboxVectorTileLayer::write_key_table()
{
    std::vector<std::string> ordered_keys(keys.size());
    for (auto const &pair : keys)
        ordered_keys[pair.second] = pair.first;

    auto const add_key_tag = [this](auto const &key) {
        layer_writer.add_string(vector_tile::KEY_TAG, key);
    };

    std::for_each(ordered_keys.begin(), ordered_keys.end(), add_key_tag);
}

void MapboxVectorTileLayer::write_value_table()
{
    std::vector<VectorTileValue> ordered_values(values.size());
    for (auto const &pair : values)
        ordered_values[pair.second] = pair.first;

    auto const call_write = [this](auto const &value) { value.write(layer_writer); };

    std::for_each(ordered_values.begin(), ordered_values.end(), call_write);
}

void MapboxVectorTileLayer::add_point(
    geometric::WGS84Coordinate const coordinate,
    std::vector<std::pair<std::string, VectorTileValue>> const &features)
{
    // only add the point, if it is within the tile
    auto const tile_point = to_tile_point(std::move(coordinate), bounding_box);
    if (!boost::geometry::within(tile_point, clip_box))
        return;

    protozero::pbf_writer feature_writer(layer_writer, vector_tile::FEATURE_TAG);
    feature_writer.add_enum(vector_tile::GEOMETRY_TAG, vector_tile::GEOMETRY_TYPE_POINT);
    feature_writer.add_uint64(vector_tile::ID_TAG, feature_id++);
    encode_features(feature_writer, features);

    // encode the point, scoping for flushing
    {
        protozero::packed_field_uint32 geometry(feature_writer,
                                                vector_tile::FEATURE_GEOMETRIES_TAG);
        geometry.add_element(vector_tile::MOVETO_COMMAND);
        geometry.add_element(protozero::encode_zigzag32(tile_point.get<0>()));
        geometry.add_element(protozero::encode_zigzag32(tile_point.get<1>()));
    }
}

void MapboxVectorTileLayer::add_line(
    std::vector<geometric::WGS84Coordinate> const &line_string,
    std::vector<std::pair<std::string, VectorTileValue>> const &features)
{
    if (line_string.size() <= 1)
        return;

    // clip the linestring to the tilebox
    Linestring unclipped_line;

    auto const append_tile_coordinate = [this,
                                         &unclipped_line](geometric::WGS84Coordinate coordinate) {
        auto const tile_point = to_tile_point(std::move(coordinate), bounding_box);
        boost::geometry::append(unclipped_line, tile_point);
    };
    std::for_each(line_string.begin(), line_string.end(), append_tile_coordinate);

    MultiLinestring clipped_line;
    boost::geometry::intersection(clip_box, unclipped_line, clipped_line);

    // only start encoding, if there is an actual line string within the box
    if (std::find_if(clipped_line.begin(), clipped_line.end(), [](auto const &line) {
            return line.size() >= 2;
        }) == clipped_line.end())
        return;

    protozero::pbf_writer feature_writer(layer_writer, vector_tile::FEATURE_TAG);
    feature_writer.add_enum(vector_tile::GEOMETRY_TAG, vector_tile::GEOMETRY_TYPE_LINE);
    feature_writer.add_uint64(vector_tile::ID_TAG, feature_id++);
    encode_features(feature_writer, features);

    for (auto const &line_segment : clipped_line)
    {
        // only encode lines
        if (line_segment.size() >= 2)
        {
            protozero::packed_field_uint32 geometry(feature_writer,
                                                    vector_tile::FEATURE_GEOMETRIES_TAG);
            geometry.add_element(vector_tile::MOVETO_COMMAND);
            std::int32_t cur_lon = 0, cur_lat = 0;
            // if this is auto, somehow it ends up as `expecting expression` to access the
            // coordinates
            auto const encode_point = [&cur_lon, &cur_lat, &geometry](IPoint const &tile_point) {
                geometry.add_element(protozero::encode_zigzag32(tile_point.get<0>() - cur_lon));
                geometry.add_element(protozero::encode_zigzag32(tile_point.get<1>() - cur_lat));
                cur_lon = tile_point.get<0>();
                cur_lat = tile_point.get<1>();
            };

            // encode the very first point
            encode_point(line_segment.front());

            // See:
            // https://github.com/mapbox/vector-tile-spec/tree/master/2.1#example-command-integers
            geometry.add_element(((line_segment.size() - 1) << 3) | 2u);
            std::for_each(line_segment.begin() + 1, line_segment.end(), encode_point);
        }
    }
}

void MapboxVectorTileLayer::encode_features(
    protozero::pbf_writer &feature_writer,
    std::vector<std::pair<std::string, VectorTileValue>> const &features)
{
    protozero::packed_field_uint32 field(feature_writer, vector_tile::FEATURE_ATTRIBUTES_TAG);

    auto const encode = [this, &field](auto const &entry) {
        auto const name_id = get_map_index(keys, entry.first);
        auto const value_id = get_map_index(values, entry.second);

        field.add_element(name_id);
        field.add_element(value_id);
    };

    std::for_each(features.begin(), features.end(), encode);
}

MapboxVectorTile::MapboxVectorTile(std::uint32_t const horizontal,
                                   std::uint32_t const vertical,
                                   std::uint32_t const zoom_level)
    : tile_writer(pbf_buffer), bounding_box(horizontal, vertical, zoom_level)
{
}

MapboxVectorTile::operator std::string() const { return pbf_buffer; }

MapboxVectorTileLayer MapboxVectorTile::new_layer(std::string const &name)
{
    return MapboxVectorTileLayer(tile_writer, name, bounding_box);
}

} // namespace container
} // namespace tool
} // namespace transit
