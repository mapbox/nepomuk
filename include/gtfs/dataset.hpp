#ifndef TRANSIT_GTFS_DATASET_HPP_
#define TRANSIT_GTFS_DATASET_HPP_

#include <bitset>
#include <cstdint>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include "geometric/coordinate.hpp"
#include "gtfs/agency.hpp"
#include "gtfs/calendar.hpp"
#include "gtfs/fare.hpp"
#include "gtfs/feed_info.hpp"
#include "gtfs/frequency.hpp"
#include "gtfs/route.hpp"
#include "gtfs/schedule.hpp"
#include "gtfs/shape.hpp"
#include "gtfs/stop.hpp"
#include "gtfs/transfer.hpp"
#include "gtfs/trip.hpp"
#include "id/service.hpp"

#include "tool/container/dictionary.hpp"
#include "tool/container/indexed_vector.hpp"
#include "tool/io/serialisation.hpp"
#include "tool/io/stream_errors.hpp"

// definitions according to https://developers.google.com/transit/gtfs/reference

namespace transit
{
namespace gtfs
{

struct Dataset
{
    std::vector<Agency> agencies;
    std::vector<Stop> stops;
    std::vector<Route> routes;
    std::vector<Trip> trips;
    std::vector<StopTime> stop_times;
    std::vector<WeeklySchedule> schedules;

    // optional additional files
    boost::optional<std::vector<CalendarDate>> calendar_dates;
    boost::optional<std::vector<FareAttribute>> fare_attributes;
    boost::optional<std::vector<FareRule>> fare_rules;
    boost::optional<std::vector<Shape>> shapes;
    boost::optional<FeedInfo> feed_info;
    boost::optional<std::vector<Transfer>> transfers;
    boost::optional<std::vector<Frequency>> frequencies;

    tool::container::Dictionary dictionary;

    // make sure all basic gtfs features are sorted by id, since we want to access stuff by its id
    void ensure_ordered();

    // not all stops in the stos.txt are reachable entities. To ensure high quality of our look-up /
    // connectivity between stops, we can filter out any unreachbale stops before creating further
    // structures from the dataset. Stops, stop_times, and transfers are being rehashed for their
    // stop-ids
    void filter_unreachable_stops();

    // https://github.com/mapbox/directions-transit/issues/73
    // Some stops offer the same name and very similar locations. In some feeds, they aren't grouped
    // into stations. This function allows to pass over the available stops and set parent stations
    // for stops that are close to each other and offer the same name.
    void connect_stops_into_stations(std::uint32_t const proximity_requirement);

    // for easy access into the geometry of routes, this function converts the internal shape
    // information into an indexed vector of coordinates
    tool::container::IndexedVector<geometric::WGS84Coordinate> shapes_as_indexed_vector();
};

template <class result_type, class converter_operator, class csv_decoder>
std::vector<result_type> decodeDataFromCSV(converter_operator converter, csv_decoder decoder)
{
    std::vector<result_type> results;
    std::vector<std::string> values;
    while (true)
    {
        tool::io::deserialize(values, decoder);
        // past eof
        if (values.empty())
            break;

        results.push_back(converter(decoder.header, values));
    }
    return results;
}

template <class result_type, class converter_operator, class csv_decoder>
std::vector<result_type> decodeDataFromCSV(converter_operator converter,
                                           csv_decoder decoder,
                                           tool::container::Dictionary &dictionary)
{
    std::vector<result_type> results;
    std::vector<std::string> values;
    while (true)
    {
        tool::io::deserialize(values, decoder);
        // past eof
        if (values.empty())
            break;

        results.push_back(converter(decoder.header, values, dictionary));
    }
    return results;
}

} // namespace gtfs
} // namespace transit

#endif // TRANSIT_GTFS_DATASET_HPP_
