#include "annotation/line_factory.hpp"

#include "gtfs/route.hpp"
#include "gtfs/trip.hpp"

#include <algorithm>

namespace nepomuk
{
namespace annotation
{

Line LineFactory::produce(std::vector<gtfs::Route> const &routes,
                          std::vector<gtfs::Trip> const &trips,
                          std::vector<TripID> const &internal_to_external_trip_id)
{
    Line line_annotation;

    auto const compute_line_info = [&](auto const external_trip_id) {
        LineInfo line_info;
        auto const &trip = trips[external_trip_id.base()];
        line_info.headsign = trip.headsign;

        auto const &route = routes[trip.route_id.base()];
        line_info.name = route.long_name;
        line_info.abbreviation = route.short_name;
        line_info.background_color = route.color;
        line_info.text_color = route.text_color;
        return line_info;
    };

    line_annotation.line_information.reserve(internal_to_external_trip_id.size());

    std::transform(internal_to_external_trip_id.begin(),
                   internal_to_external_trip_id.end(),
                   std::back_inserter(line_annotation.line_information),
                   compute_line_info);
    return line_annotation;
}

} // namespace annotation
} // namespace nepomuk
