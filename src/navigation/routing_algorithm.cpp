#include "navigation/routing_algorithm.hpp"

#include <algorithm>
#include <boost/assert.hpp>

namespace transit
{
namespace navigation
{

RoutingAlgorithm::RoutingAlgorithm(std::vector<timetable::LineTable> const &line_tables)
    : line_tables(line_tables)
{
}

void RoutingAlgorithm::update_departures_and_arrivals(std::vector<PathEntry> &path) const
{
    auto const shift_parent_depature_to_front = [](auto &lhs, auto &rhs) {
        lhs.departure = rhs.departure;
        // we never find anything in this search :(
        return false;
    };

    std::adjacent_find(path.begin(), path.end(), shift_parent_depature_to_front);
    path.back().departure = path.back().arrival;
}

Trip RoutingAlgorithm::make_trip(std::vector<PathEntry> &path) const
{
    Trip result;
    Leg leg;
    // we only know the first line, after starting. We always reach the first station via walking
    // transfer. To avoid an additional leg here, we set the line to the line of the second station
    update_departures_and_arrivals(path);

    BOOST_ASSERT(path[0].line == WALKING_TRANSFER);
    if (path.size() > 1)
        path[0].line = path[1].line;

    path = unpack_path(std::move(path));

    set_departure(leg, path[0].departure);
    set_line(leg, path[0].line);

    auto current_line = path[0].line;
    for (auto itr = path.begin(); itr != path.end(); ++itr)
    {
        if (itr->line != current_line)
        {
            add_leg(result, std::move(leg));
            leg = Leg();
            current_line = itr->line;
            if (current_line == WALKING_TRANSFER && itr + 1 != path.end())
                current_line = (itr + 1)->line;

            set_departure(leg, (itr - 1)->departure);
            set_line(leg, current_line);

            // only repeat last stop, if it wasn't a walking transfer
            if (itr->line != WALKING_TRANSFER)
                add_stop(leg, Leg::stop_type{(itr - 1)->stop, (itr - 1)->arrival});
        }
        add_stop(leg, Leg::stop_type{itr->stop, itr->arrival});
    }
    // add the final leg
    add_leg(result, std::move(leg));
    return result;
}

std::vector<RoutingAlgorithm::PathEntry>
RoutingAlgorithm::unpack_path(std::vector<PathEntry> packed_path) const
{
    std::vector<PathEntry> unpacked_path;

    auto const add_intermediate_stops = [&](auto const &from, auto const &to) {
        unpacked_path.push_back(from);

        // add stops
        if (to.line != WALKING_TRANSFER)
        {
            auto const &stops = line_tables[to.line.base()].stops();
            auto range = stops.list(from.stop);
            if (!range.empty())
            {
                for (auto itr = range.begin() + 1; itr != range.end(); ++itr)
                {
                    auto const s = *itr;
                    if (s == to.stop)
                        break;
                    unpacked_path.push_back({s, to.line, {}, {}});
                }
            }
        }
        // for adjacent find
        return false;
    };

    std::adjacent_find(packed_path.begin(), packed_path.end(), add_intermediate_stops);
    unpacked_path.push_back(packed_path.back());

    return unpacked_path;
}

void RoutingAlgorithm::add_leg(Trip &trip, Leg leg) const { trip.legs.push_back(std::move(leg)); }

void RoutingAlgorithm::add_stop(Leg &leg, Leg::stop_type stop) const
{
    leg.stops.push_back(std::move(stop));
}

void RoutingAlgorithm::set_departure(Leg &leg, date::Time time) const { leg._departure = time; }

void RoutingAlgorithm::set_line(Leg &leg, LineID line) const { leg._line = line; }

} // namespace navigation
} // namespace transit
