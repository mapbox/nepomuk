#include "geometric/polyline.hpp"

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <sstream>

namespace transit
{
namespace geometric
{

namespace
{

constexpr const std::uint32_t bits_in_chunk = 5;
constexpr const std::uint32_t continuation_bit = 1 << bits_in_chunk;
constexpr const std::uint32_t chunk_mask = (1 << bits_in_chunk) - 1;
constexpr const std::uint32_t bigger_chunk_mask = (1 << (bits_in_chunk + 1)) - 1;

std::string encode_deltas(std::vector<std::int32_t> &delta_values)
{
    auto const prepare_for_encode = [](std::int32_t value) {
        if (value < 0)
        {
            const unsigned binary = std::llabs(value);
            const unsigned twos = (~binary) + 1u; // two complement
            const unsigned shl = twos << 1u;
            return static_cast<std::int32_t>(~shl);
        }
        else
            return value << 1u;
    };

    std::transform(
        delta_values.begin(), delta_values.end(), delta_values.begin(), prepare_for_encode);

    std::ostringstream oss;
    auto const encode_int = [&oss](std::int32_t value) {
        while (value >= 0x20)
        {
            const std::int32_t next_value = (0x20 | (value & 0x1f)) + bigger_chunk_mask;
            oss << static_cast<char>(next_value);
            value >>= bits_in_chunk;
        }

        value += bigger_chunk_mask;
        oss << static_cast<char>(value);
    };

    std::for_each(delta_values.begin(), delta_values.end(), encode_int);
    return oss.str();
}
} // namespace

std::string Polyline::encode(std::uint32_t const precision,
                             std::vector<WGS84Coordinate> const &coordinates)
{
    if (coordinates.empty())
        return {};

    std::int32_t current_lat = 0, current_lon = 0;
    std::vector<std::int32_t> delta_numbers(2 * coordinates.size(), 0);

    // pre-allocated, so we can fake a back_inserter iterator
    auto back_inserter = delta_numbers.begin();

    auto add_coordinate_as_delta = [&](auto const coordinate) {
        auto lat = static_cast<std::int32_t>(
            std::llround(doubleFromLatLon(coordinate.latitude) * precision));
        auto lon = static_cast<std::int32_t>(
            std::llround(doubleFromLatLon(coordinate.longitude) * precision));
        auto const delta_lat = lat - current_lat;
        auto const delta_lon = lon - current_lon;
        *back_inserter++ = delta_lat;
        *back_inserter++ = delta_lon;
        current_lat = lat;
        current_lon = lon;
    };

    std::for_each(coordinates.begin(), coordinates.end(), add_coordinate_as_delta);

    return encode_deltas(delta_numbers);
}

std::vector<WGS84Coordinate> Polyline::decode(std::uint32_t precision, std::string const &polyline)
{
    // https://developers.google.com/maps/documentation/utilities/polylinealgorithm
    auto decode_polyline_integer = [](auto &first, auto last) {
        // varint coding parameters

        std::uint32_t result = 0;
        for (std::uint32_t value = continuation_bit, shift = 0;
             (value & continuation_bit) && (shift < CHAR_BIT * sizeof(result) - 1) && first != last;
             ++first, shift += bits_in_chunk)
        {
            value = *first - 63; // convert ASCII coding [?..~] to an integer [0..63]
            result |= (value & chunk_mask) << shift;
        }

        // change "zig-zag" sign coding to two's complement
        result = ((result & 1) == 1) ? ~(result >> 1) : (result >> 1);
        return static_cast<std::int32_t>(result);
    };

    auto polyline_to_coordinate = [precision](auto value) {
        return static_cast<double>(value) / precision;
    };

    std::vector<geometric::WGS84Coordinate> coordinates;
    std::int32_t latitude = 0, longitude = 0;

    std::string::const_iterator first = polyline.begin();
    const std::string::const_iterator last = polyline.end();
    while (first != last)
    {
        const auto dlat = decode_polyline_integer(first, last);
        const auto dlon = decode_polyline_integer(first, last);

        latitude += dlat;
        longitude += dlon;

        coordinates.emplace_back(geometric::WGS84Coordinate{
            geometric::makeLatLonFromDouble<FixedLongitude>(polyline_to_coordinate(longitude)),
            geometric::makeLatLonFromDouble<FixedLatitude>(polyline_to_coordinate(latitude))});
    }

    return coordinates;
}

} // namespace geometric
} // namespace transit
