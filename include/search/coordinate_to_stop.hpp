#ifndef TRANSIT_SEARCH_COORDINATE_TO_STOP_HPP_
#define TRANSIT_SEARCH_COORDINATE_TO_STOP_HPP_

#include "geometric/coordinate.hpp"
#include "gtfs/stop.hpp"

#include <utility>
#include <vector>

namespace transit
{
namespace search
{

class CoordinateToStop
{
  public:
    CoordinateToStop(std::vector<std::pair<geometric::Coordinate, gtfs::StopID>>);

    // find the nearest stop to a location
    gtfs::StopID nearest(geometric::Coordinate const &location) const;

  private:
    // TODO add an overlay to find the correct coordinates more quickly
    std::vector<std::pair<geometric::Coordinate, gtfs::StopID>> coordinates;
};

} // namespace search
} // namespace transit

#endif // TRANSIT_SEARCH_COORDINATE_TO_STOP_HPP_
