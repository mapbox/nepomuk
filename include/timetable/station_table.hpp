#ifndef NEPOMUK_TIMETABLE_STATION_TABLE_HPP_
#define NEPOMUK_TIMETABLE_STATION_TABLE_HPP_

#include <cstddef>

#include "id/stop.hpp"
#include "tool/container/surjective_mapping.hpp"

namespace nepomuk
{
namespace timetable
{

// a wrapper class for a surjective mapping, mapping stations to stops and back
class StationTable
{
  public:
    using Lookup = tool::container::SurjectiveMapping<StopID, StopID>;

    StationTable(Lookup stop_station_lookup);
    StationTable();

    // forward the wrapper mappings
    auto stops(StopID const station_id) const
    {
        return stop_station_lookup.domain_mapping(station_id);
    }
    auto station(StopID const stop_id) const
    {
        return stop_station_lookup.codomain_mapping(stop_id);
    }

    std::size_t num_stops() const;
    std::size_t num_stations() const;

  private:
    Lookup stop_station_lookup;
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_STATION_TABLE_HPP_
