#include "timetable/station_table_factory.hpp"
#include "timetable/exceptions.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <utility>

namespace nepomuk
{
namespace timetable
{

namespace
{
template <typename iterator_type>
void check_input_validity(iterator_type const begin, iterator_type const end)
{
    std::uint64_t const num_stations = std::distance(begin, end);

    // check if the domain ids match the domain size
    auto const invalid_domains = [num_stations](auto const &stop) {
        auto const invalid_id = static_cast<std::uint64_t>(stop.id) >= num_stations;
        auto const invalid_parent =
            stop.parent_station && static_cast<std::uint64_t>(*stop.parent_station) >= num_stations;
        return invalid_id || invalid_parent;
    };

    // check if in range
    if (end != std::find_if(begin, end, invalid_domains))
        throw InvalidInputError("Domain keys don't match domain size");
}
} // namespace

StationTable StationTableFactory::produce(std::vector<gtfs::Stop>::iterator const begin,
                                          std::vector<gtfs::Stop>::iterator const end)
{
    // check the domain size
    check_input_validity(begin, end);

    auto const number_of_stops = std::distance(begin, end);

    StationTable::Lookup lookup(number_of_stops, number_of_stops);

    auto map_stop = [&lookup](auto const &stop) {
        // a stop is either mapped to a parent station or is a station itself
        auto const station_id = stop.parent_station ? *stop.parent_station : stop.id;
        lookup.map_codomain(stop.id, station_id);
    };

    // construct the mapping for the stop table
    std::for_each(begin, end, map_stop);

    return StationTable(std::move(lookup));
}

} // namespace timetable
} // namespace nepomuk
