#include "search/stop_to_line_factory.hpp"
#include "timetable/line_table.hpp"

#include "id/line.hpp"              // for LineID
#include "id/stop.hpp"              // for StopID
#include "timetable/stop_table.hpp" // for StopTable

#include <algorithm>

namespace transit
{
namespace search
{

StopToLine StopToLineFactory::produce(std::size_t const num_stops,
                                      std::vector<timetable::LineTable> const &line_tables)
{
    StopToLine look_up;

    look_up.trip_table.resize(num_stops);
    for (std::size_t line_id = 0; line_id < line_tables.size(); ++line_id)
    {
        // get stops of the line
        auto const &stop_table = line_tables[line_id].stop_table;
        auto const add_to_lookup = [&look_up, line_id](auto const stop_id) {
            look_up.trip_table[stop_id.base()].push_back(LineID{line_id});
        };
        std::for_each(stop_table.stops.begin(), stop_table.stops.end(), add_to_lookup);
    }

    return look_up;
}

} // namespace search
} // namespace transit
