#ifndef NEPOMUK_TIMETABLE_LINETABLE_HPP_
#define NEPOMUK_TIMETABLE_LINETABLE_HPP_

#include "date/time.hpp"
#include "id/stop.hpp"
#include "timetable/departure_table.hpp"
#include "timetable/duration_table.hpp"
#include "timetable/stop_table.hpp"

#include <boost/optional.hpp>
#include <vector>

namespace nepomuk
{
namespace search
{
class StopToLineFactory;
} // namespace search

namespace timetable
{

// A line table describes a line within the public transit network. It consists of a single entry of
// stops serviced along the line. The line allows a series of departures and a list of arrivals that
// can be computed via delta times relative to the departure at the very first station. Within a
// line, there cannot be any overtaking. A vehicle departing at time T will always arrive at the
// last stop of the line before any vehicle departing at T + x
class LineTable
{
  public:
    struct Trip
    {
        StopTable::const_iterator_range const stop_range;
        DurationTable::const_iterator_range const duration_range;
        date::Time const departure;
    };

    // give access to the next departure of the given line
    boost::optional<Trip> get(StopID const stop, date::Time const departure) const;

    using stop_iterator = StopTable::const_iterator;
    using stop_iterator_range = StopTable::const_iterator_range;

    StopTable const &stops() const;

  private:
    // the list of arrivals of a line
    StopTable stop_table;
    std::vector<DurationTable> duration_tables;
    // the time delta required to reach stop_x (maximum), to get a good impression on what the
    // desired base departure should be like for a time T (T-td -> departure table)
    std::vector<std::uint32_t> time_deltas;

    // the list of departure times available
    DepartureTable departures;

    friend class LineTableFactory;
    friend class nepomuk::search::StopToLineFactory;
};

} // namespace timetable
} // namespace nepomuk

#endif // NEPOMUK_TIMETABLE_LINETABLE_HPP_
