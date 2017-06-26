#include "annotation/geometry_factory.hpp"

#include "gtfs/stop.hpp"
#include "gtfs/trip.hpp"
#include "id/line.hpp"
#include "id/shape.hpp"
#include "id/stop.hpp"
#include "timetable/trip_table.hpp"
#include "tool/status/progress.hpp"

#include <algorithm>
#include <boost/assert.hpp>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <set>

namespace nepomuk
{
namespace annotation
{

Geometry GeometryFactory::produce(timetable::TripTable const &trip_table,
                                  std::vector<TripID> const &internal_to_external_trip_mapping,
                                  std::vector<gtfs::Stop> const &stops,
                                  std::vector<gtfs::Trip> const &trips,
                                  timetable::SegmentTable const &segment_table)
{
    tool::status::FunctionTimingGuard timing_guard("Geometry Annotation Creation");
    Geometry geometry_annotation(segment_table);

    // ensure geometry for all stops is stored
    geometry_annotation.stop_locations.reserve(stops.size());
    std::transform(stops.begin(),
                   stops.end(),
                   std::back_inserter(geometry_annotation.stop_locations),
                   [](auto const &stop) { return stop.location; });

    // for all trip ids, remember their geometry id
    geometry_annotation.shape_by_trip.reserve(internal_to_external_trip_mapping.size());
    geometry_annotation.geometry_offsets.resize(trip_table.all_stops.size());

    std::set<std::size_t> handled_offsets;

    // match the geometry to the stop range by finding stop locations within the geometry
    auto const map_geometries = [&](auto output_itr,
                                    auto stop_begin,
                                    auto const stop_end,
                                    auto geometry_begin,
                                    auto const geometry_end) {
        auto candidate_begin = geometry_begin;
        // remember all offsets along the stop range
        for (; stop_begin != stop_end; ++stop_begin)
        {
            auto const stop = *stop_begin;
            auto itr = std::min_element(
                candidate_begin, geometry_end, [&](auto const lhs, auto const rhs) {
                    return geometric::distance(stops[stop.base()].location, lhs) <
                           geometric::distance(stops[stop.base()].location, rhs);

                });
            candidate_begin = itr;
            *output_itr++ = std::distance(geometry_begin, itr);
        }
    };

    auto const generate_geometry_entries =
        [&,
         mapped_id_itr = internal_to_external_trip_mapping.begin() ](auto const &departure) mutable
    {
        // get the current trip id
        auto const mapped_id = *mapped_id_itr++;
        auto const maybe_shape_id = trips[mapped_id.base()].shape_id;
        // remember the shape of each internal trip, which is an offset into the segment_table
        geometry_annotation.shape_by_trip.push_back(maybe_shape_id);

        // when there is a shape, we can try and set the offsets for the shape in the stop array
        if (maybe_shape_id && !handled_offsets.count(departure.stop_offset))
        {
            // prevent doing work multiple times
            handled_offsets.insert(departure.stop_offset);

            // access into the shapes
            auto range = segment_table.crange(maybe_shape_id->base());

            auto const begin_stops = trip_table.all_stops.begin() + departure.stop_offset;
            auto const end_stops = std::find(trip_table.all_stops.begin() + departure.stop_offset,
                                             trip_table.all_stops.end(),
                                             StopID::INVALID());

            map_geometries(geometry_annotation.geometry_offsets.begin() + departure.stop_offset,
                           begin_stops,
                           end_stops,
                           range.begin(),
                           range.end());
        }
    };

    std::for_each(
        trip_table.departures.begin(), trip_table.departures.end(), generate_geometry_entries);

    return geometry_annotation;
}

} // namespace annotation
} // namespace nepomuk
