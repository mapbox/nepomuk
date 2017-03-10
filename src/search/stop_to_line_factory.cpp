#include "search/stop_to_line_factory.hpp"

#include <algorithm>

namespace transit
{
namespace search
{

StopToLine StopToLineFactory::produce(std::size_t const num_stops,
                                      std::vector<gtfs::StopTime> const &stop_times)
{
    StopToLine look_up;

    look_up.trip_table.resize(num_stops);

    auto const add_to_lookup = [&look_up](auto const &stop_time) {
        look_up.trip_table[static_cast<std::uint64_t>(stop_time.stop_id)].push_back(stop_time.trip_id);
    };

    std::for_each(stop_times.begin(), stop_times.end(), add_to_lookup);

    return look_up;
}

} // namespace search
} // namespace transit
