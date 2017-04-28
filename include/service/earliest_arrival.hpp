#ifndef TRANSIT_SERVICE_EARLIEST_ARRIVAL_HPP_
#define TRANSIT_SERVICE_EARLIEST_ARRIVAL_HPP_

#include "annotation/osrm.hpp"
//#include "navigation/algorithm/timetable.hpp"
#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "search/coordinate_to_stop.hpp"
#include "service/interface.hpp"
#include "service/master.hpp"

namespace transit
{
namespace service
{

class EarliestArrival : public Interface
{
  public:
    EarliestArrival(service::Master &master_service);
    ~EarliestArrival() final override = default;

    ServiceStatus operator()(ServiceParameters &parameters) const final override;

  private:
    annotation::OSRM const &annotation;
    search::CoordinateToStop const &coordinate_to_stop;
    navigation::algorithm::TimeTableDijkstra navigator;
    // navigation::algorithm::TimeTable navigator;
};

} // namespace service
} // namespace transit

#endif // TRANSIT_SERVICE_EARLIEST_ARRIVAL_HPP_
