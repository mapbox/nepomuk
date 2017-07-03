#ifndef NEPOMUK_TIMETABLE_TRIP_TABLE_FACTORY_HPP
#define NEPOMUK_TIMETABLE_TRIP_TABLE_FACTORY_HPP

#include "gtfs/frequency.hpp"
#include "gtfs/schedule.hpp"
#include "gtfs/stop.hpp"

#include "timetable/stop_table.hpp"
#include "timetable/trip_table.hpp"

#include "id/trip.hpp"

#include <boost/optional.hpp>
#include <unordered_map>
#include <vector>

namespace nepomuk
{
namespace timetable
{

class TripTableFactory
{
  public:
    TripTableFactory(std::int32_t utc_offset);

    // in case frequencies are not specified, we detect trips from the stop_time tables
    TripID produce(std::vector<gtfs::StopTime>::iterator begin,
                   std::vector<gtfs::StopTime>::iterator end);

    // extract the produced result from the factory
    TripTable extract()
    {
        std::cout << table.all_stops.size() << " Stops. " << table.all_durations.size()
                  << std::endl;
        return std::move(table);
    }
    auto get_mapping() { return std::move(external_from_internal_trip_id); }

  private:
    TripTable table;

    // keep track of which trip ids the entries in the trip table represent (since the trips might
    // contain holes in the ID space)
    std::vector<TripID> external_from_internal_trip_id;
    std::int32_t utc_offset;

    //
    // Handling of stops
    //
    std::unordered_map<StopID, std::vector<std::size_t>> stop_locations_stops;

    // handle stops. Add them to a list of all stops, remember one offset per trip
    std::size_t produceStopOffset(std::vector<gtfs::StopTime>::iterator begin,
                                  const std::vector<gtfs::StopTime>::iterator end);

    boost::optional<std::size_t> findStopOffset(const std::vector<gtfs::StopTime>::iterator begin,
                                                const std::vector<gtfs::StopTime>::iterator end);

    //
    // Handling of Durations
    //

    std::unordered_map<std::uint32_t, std::vector<std::size_t>> locations_durations;

    boost::optional<std::size_t>
    findDurationOffset(const std::vector<std::uint32_t>::iterator begin,
                       const std::vector<std::uint32_t>::iterator end);

    std::size_t produceDurationOffset(std::vector<gtfs::StopTime>::iterator begin,
                                      const std::vector<gtfs::StopTime>::iterator end);
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_TRIP_TABLE_FACTORY_HPP
