#include "timetable/timetable.hpp"

namespace transit
{
namespace timetable
{

LineTable const &TimeTable::line(LineID const &line) const { return line_tables[line.base()]; }

TransferTable::iterator_range TimeTable::transfers(gtfs::StopID const stop) const
{
    return transfer_table.get(stop);
}

} // namespace timetable
} // namespace transit
