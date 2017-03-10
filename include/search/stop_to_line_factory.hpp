#ifndef TRANSIT_SEARCH_STOP_TO_LINE_FACTORY_HPP_
#define TRANSIT_SEARCH_STOP_TO_LINE_FACTORY_HPP_

#include "gtfs/stop.hpp"
#include "search/stop_to_line.hpp"

#include <cstddef>
#include <vector>

namespace transit
{
namespace search
{

class StopToLineFactory
{
  public:
    // build a look-up from stop-id to trip-id
    static StopToLine produce(std::size_t const num_stops,
                              std::vector<gtfs::StopTime> const &stop_times);
};

} // namespace search
} // namespace transit

#endif // TRANSIT_SEARCH_STOP_TO_LINE_FACTORY_HPP_
