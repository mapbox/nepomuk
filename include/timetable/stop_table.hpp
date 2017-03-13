#ifndef TRANSIT_TIMETABLE_STOPTABLE_HPP_
#define TRANSIT_TIMETABLE_STOPTABLE_HPP_

#include <cstdint>
#include <vector>

#include "gtfs/stop.hpp"
#include "gtfs/trip.hpp"

#include <boost/range/iterator_range.hpp>

namespace transit
{

namespace search
{
class StopToLineFactory;
} // namespace search

namespace timetable
{

class StopTableFactory;

// For any given line, the stop table defines the list of stops that can be reached along the line
// and the time from a given departure that is required to do so. It groups all entries that follow
// the same stopping pattern. If patterns diverge, the table requires two different instantiations.
class StopTable
{
  public:
    struct Arrival
    {
        // the ID of the stop / station
        gtfs::StopID stop_id;
        // the time along the line to reach the station
        std::uint32_t delta_t;

        bool operator==(Arrival const &other) const;
    };

    using const_iterator = std::vector<Arrival>::const_iterator;
    using const_iterator_range = boost::iterator_range<const_iterator>;

    // list stations along the line, and their departure
    const_iterator_range list(gtfs::StopID start_id) const;
    bool operator==(StopTable const &other) const;

  private:
    std::vector<Arrival> stop_arrivals;

    friend class StopTableFactory;
    friend class transit::search::StopToLineFactory;
};

} // namespace timetable
} // namespace transit

#endif // TRANSIT_TIMETABLE_STOPTABLE_HPP_
