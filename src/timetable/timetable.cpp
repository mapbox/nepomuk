#include "timetable/timetable.hpp"

namespace nepomuk
{
namespace timetable
{

TransferTable::iterator_range TimeTable::transfers(StopID const stop) const
{
    return transfer_table.get(stop);
}

} // namespace timetable
} // namespace nepomuk
