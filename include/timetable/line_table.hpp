#ifndef TRANSIT_TIMETABLE_LINETABLE_HPP_
#define TRANSIT_TIMETABLE_LINETABLE_HPP_

#include "gtfs/time.hpp"
#include "timetable/departure_table.hpp"
#include "timetable/duration_table.hpp"
#include "timetable/stop_table.hpp"

#include "tool/types_and_traits/strong_typedef.hpp"
#include <boost/optional.hpp>
#include <vector>

namespace transit
{
namespace search
{
class StopToLineFactory;
} // namespace search

namespace timetable
{

STRONG_ID_TYPE(std::uint64_t, LineID)

class LineTableFactory;

// A line table describes a line within the public transit network. It consists of a single entry of
// stops serviced along the line. The line allows a series of departures and a list of arrivals that
// can be computed via delta times relative to the departure at the very first station.
class LineTable
{
  public:
    struct Trip
    {
        StopTable const &stop_table;
        DurationTable const &duration_table;
        gtfs::Time const departure;

        Trip(StopTable const &stop_table,
             DurationTable const &duration_table,
             gtfs::Time const time);
    };

    // give access to the next departure of the given line
    boost::optional<Trip> get(gtfs::Time const departure) const;

    using stop_iterator = StopTable::const_iterator;
    using stop_iterator_range = StopTable::const_iterator_range;

    stop_iterator_range list_stops() const;

  private:
    // the list of arrivals of a line
    StopTable stop_table;
    std::vector<DurationTable> duration_tables;

    // the list of departure times available
    DepartureTable departures;

    friend class LineTableFactory;
    friend class transit::search::StopToLineFactory;
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_LINETABLE_HPP_
