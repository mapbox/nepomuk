#include "timetable/timetable.hpp"

#include "id/line.hpp" // for LineID

namespace transit
{
namespace timetable
{

LineTable const &TimeTable::line(LineID const &line) const { return line_tables[line.base()]; }
std::vector<LineTable> const &TimeTable::lines() const { return line_tables; }

TransferTable::iterator_range TimeTable::transfers(StopID const stop) const
{
    return transfer_table.get(stop);
}

} // namespace timetable
} // namespace transit
