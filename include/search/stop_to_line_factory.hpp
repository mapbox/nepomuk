#ifndef NEPOMUK_SEARCH_STOP_TO_LINE_FACTORY_HPP_
#define NEPOMUK_SEARCH_STOP_TO_LINE_FACTORY_HPP_

#include "search/stop_to_line.hpp"

#include <cstddef>
#include <vector>

namespace nepomuk
{

namespace timetable
{
class LineTable;
}

namespace search
{

class StopToLineFactory
{
  public:
    // build a look-up from stop-id to trip-id
    static StopToLine produce(std::size_t const num_stops,
                              std::vector<timetable::LineTable> const &line_tables);
};

} // namespace search
} // namespace nepomuk

#endif // NEPOMUK_SEARCH_STOP_TO_LINE_FACTORY_HPP_
