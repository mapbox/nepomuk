#include "timetable/transfer_table.hpp"

namespace nepomuk
{
namespace timetable
{

TransferTable::iterator_range TransferTable::get(StopID const stop_id) const
{
    auto range_itr = index.find(stop_id);
    if (range_itr == index.end())
        return boost::make_iterator_range(transfers.end(), transfers.end());
    else
        return boost::make_iterator_range(transfers.begin() + range_itr->second.first,
                                          transfers.begin() + range_itr->second.second);
}

} // namespace timetable
} // namespace nepomuk
