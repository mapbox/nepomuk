#include "annotation/line_factory.hpp"

#include "gtfs/route.hpp"
#include "gtfs/trip.hpp"

#include <algorithm>

namespace nepomuk
{
namespace annotation
{

Line LineFactory::produce(std::vector<size_t> const &trip_offset_by_line,
                          std::vector<gtfs::Route> const &routes,
                          std::vector<gtfs::Trip> const &trips)
{
    Line line_annotation;

    auto const compute_line_info = [&](auto const trip_offset) {
        LineInfo line_info;
        line_info.headsign = trips[trip_offset].headsign;

        auto const &route = routes[trips[trip_offset].route_id.base()];
        line_info.name = route.long_name;
        line_info.abbreviation = route.short_name;
        line_info.background_color = route.color;
        line_info.text_color = route.text_color;
        return line_info;
    };

    line_annotation.line_information.reserve(trip_offset_by_line.size());
    std::transform(trip_offset_by_line.begin(),
                   trip_offset_by_line.end(),
                   std::back_inserter(line_annotation.line_information),
                   compute_line_info);

    return line_annotation;
}

} // namespace annotation
} // namespace nepomuk
