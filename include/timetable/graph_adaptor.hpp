#ifndef TRANSIT_TIMETABLE_GRAPH_ADAPTOR_HPP_
#define TRANSIT_TIMETABLE_GRAPH_ADAPTOR_HPP_

#include "search/stop_to_line.hpp"
#include "timetable/timetable.hpp"
#include "tool/container/forward_star_graph.hpp"

namespace transit
{
namespace timetable
{

class TimetableToGraphAdaptor
{
  public:
    // construct a graph representation for connectivity reasons of the timetable graph
    static tool::container::AdjacencyGraph adapt(TimeTable const &timetable,
                                                 search::StopToLine const &stop_to_line);
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_GRAPH_ADAPTOR_HPP_
