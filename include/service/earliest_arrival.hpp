#ifndef NEPOMUK_SERVICE_EARLIEST_ARRIVAL_HPP_
#define NEPOMUK_SERVICE_EARLIEST_ARRIVAL_HPP_

//#include "navigation/algorithm/timetable.hpp"
#include "annotation/api.hpp"
#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "search/coordinate_to_stop.hpp"
#include "service/interface.hpp"
#include "service/master.hpp"

namespace nepomuk
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
    annotation::API const &annotation;
    search::CoordinateToStop const &coordinate_to_stop;
    navigation::algorithm::TimeTableDijkstra navigator;
    // navigation::algorithm::TimeTable navigator;
};

} // namespace service
} // namespace nepomuk

#endif // NEPOMUK_SERVICE_EARLIEST_ARRIVAL_HPP_
