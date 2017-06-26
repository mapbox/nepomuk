#ifndef NEPOMUK_TIMETABLE_GRAPH_ADAPTOR_HPP_
#define NEPOMUK_TIMETABLE_GRAPH_ADAPTOR_HPP_

#include "tool/container/forward_star_graph.hpp"

namespace nepomuk
{

namespace search
{
class StopToLine;
}

namespace timetable
{

class TimeTable;
class StopToTrip;

class TimetableToGraphAdaptor
{
  public:
    // construct a graph representation for connectivity reasons of the timetable graph
    static tool::container::AdjacencyGraph adapt(TimeTable const &timetable,
                                                 timetable::StopToTrip const &stop_to_trip);
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_GRAPH_ADAPTOR_HPP_
