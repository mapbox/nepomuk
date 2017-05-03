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

Route RoutingAlgorithm::make_route(std::vector<PathEntry> path) const
{
    // currently we only support a single leg
    Route result;
    Leg leg;
    Connection connection;

    auto const make_segment = [](auto type, auto container) {
        Segment segment;
        segment._type = std::move(type);
        segment.container = std::move(container);
        return segment;
    };

    // really hacked version, we are not travelling in 1970
    auto const to_utc = [](auto date) { return date::UTCTimestamp(date.seconds_since_midnight); };

    // we only know the first line, after starting. We always reach the first station via walking
    // transfer. To avoid an additional leg here, we set the line to the line of the second station
    update_departures_and_arrivals(path);

    BOOST_ASSERT(path[0].line == WALKING_TRANSFER);
    if (path.size() > 1)
        path[0].line = path[1].line;

    path = unpack_path(std::move(path));

    auto segment = make_segment(SegmentType::TRANSIT, segment::Transit());
    set_line(connection, path[0].line);
    set_departure(connection, to_utc(path[0].departure));

    auto current_line = path[0].line;
    for (auto itr = path.begin(); itr != path.end(); ++itr)
    {
        if (itr->line != current_line)
        {
            add_connection(segment.as_transit(), std::move(connection));
            connection = Connection();
            add_segment(leg, std::move(segment));

            if (itr->line == WALKING_TRANSFER && itr + 1 != path.end())
            {
                segment = make_segment(SegmentType::TRANSFER, segment::Transfer());
                auto &transfer = segment.as_transfer();
                set_departure(transfer, to_utc(itr->arrival));
                set_arrival(transfer, to_utc(itr->departure));
                transfer._origin = itr->stop;
                transfer._destination = itr->stop;
                current_line = (itr + 1)->line;
                add_segment(leg, std::move(segment));
            }
            else
            {
                current_line = itr->line;
            }
            set_line(connection, current_line);
            set_departure(connection, to_utc((itr - 1)->departure));

            segment = make_segment(SegmentType::TRANSIT, segment::Transit());

            // only repeat last stop, if it wasn't a walking transfer
            if (itr->line != WALKING_TRANSFER)
            {
                Stop previous;
                previous._id = (itr - 1)->stop;
                previous._departure = to_utc((itr - 1)->departure);
                previous._arrival = to_utc((itr - 1)->departure);
                add_stop(segment.as_transit(), previous);
            }
        }
        Stop stop;
        set_arrival(stop, to_utc(itr->arrival));
        set_departure(stop, to_utc(itr->departure));
        stop._id = itr->stop;
        add_stop(segment.as_transit(), stop);
        set_arrival(connection, to_utc(itr->arrival));
    }
    // add the final leg
    add_connection(segment.as_transit(), std::move(connection));
    add_segment(leg, std::move(segment));
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
                    unpacked_path.push_back({s, to.line, from.departure, from.departure});
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

void RoutingAlgorithm::add_leg(Route &route, Leg leg) const
{
    route._legs.push_back(std::move(leg));
}

void RoutingAlgorithm::add_stop(segment::Transit &segment, Stop stop) const
{
    segment._stops.push_back(std::move(stop));
}

void RoutingAlgorithm::add_connection(segment::Transit &segment, Connection connection) const
{
    segment._connections.push_back(std::move(connection));
}

void RoutingAlgorithm::add_segment(Leg &leg, Segment segment) const
{
    leg._segments.push_back(std::move(segment));
}

void RoutingAlgorithm::set_departure(Stop &stop, date::UTCTimestamp time) const
{
    stop._departure = time;
}
void RoutingAlgorithm::set_arrival(Stop &stop, date::UTCTimestamp time) const
{
    stop._arrival = time;
}
void RoutingAlgorithm::set_departure(Connection &connection, date::UTCTimestamp time) const
{
    connection._departure = time;
}
void RoutingAlgorithm::set_arrival(Connection &connection, date::UTCTimestamp time) const
{
    connection._arrival = time;
}

void RoutingAlgorithm::set_departure(segment::Transfer &segment, date::UTCTimestamp time) const
{
    segment._departure = time;
}
void RoutingAlgorithm::set_arrival(segment::Transfer &segment, date::UTCTimestamp time) const
{
    segment._arrival = time;
}
void RoutingAlgorithm::set_origin(segment::Transfer &transfer, StopID stop) const
{
    transfer._origin = std::move(stop);
}
void RoutingAlgorithm::set_destination(segment::Transfer &transfer, StopID stop) const
{
    transfer._destination = std::move(stop);
}
void RoutingAlgorithm::set_departure(segment::Walk &segment, date::UTCTimestamp time) const
{
    segment._departure = time;
}
void RoutingAlgorithm::set_arrival(segment::Walk &segment, date::UTCTimestamp time) const
{
    segment._arrival = time;
}

void RoutingAlgorithm::set_line(Connection &connection, LineID line) const
{
    connection._line = line;
}

Segment RoutingAlgorithm::make_segment(segment::Transfer transfer) const
{
    Segment segment;
    segment._type = SegmentType::TRANSFER;
    segment.container = transfer;
    return segment;
}

Segment RoutingAlgorithm::make_segment(segment::Transit transit) const
{
    Segment segment;
    segment._type = SegmentType::TRANSIT;
    segment.container = transit;
    return segment;
}

Segment RoutingAlgorithm::make_segment(segment::Walk walk) const
{
    Segment segment;
    segment._type = SegmentType::WALK;
    segment.container = walk;
    return segment;
}

Stop RoutingAlgorithm::make_stop(StopID const id,
                                 date::UTCTimestamp const arrival,
                                 date::UTCTimestamp const departure) const
{
    Stop stop;
    stop._id = id;
    stop._arrival = arrival;
    stop._departure = departure;
    return stop;
}

Connection RoutingAlgorithm::make_connection(LineID const id,
                                             date::UTCTimestamp const departure,
                                             date::UTCTimestamp const arrival) const
{
    Connection connection;
    connection._line = id;
    connection._departure = departure;
    connection._arrival = arrival;
    return connection;
}

} // namespace navigation
} // namespace transit
