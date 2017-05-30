#ifndef NEPOMUK_SERVICE_ROUTE_HPP_
#define NEPOMUK_SERVICE_ROUTE_HPP_

//#include "navigation/algorithm/timetable.hpp"
#include "annotation/api.hpp"
#include "navigation/algorithm/timetable_dijkstra.hpp"
#include "search/coordinate_to_stop.hpp"
#include "service/master.hpp"
#include "service/route_parameters.hpp"

#include <vector>

namespace nepomuk
{

namespace navigation
{
class Route;
} // namespace navigation

namespace service
{

class Route
{
  public:
    Route(service::Master &master_service);

    std::vector<navigation::Route> operator()(RouteParameters &parameters) const;

  private:
    search::CoordinateToStop const &coordinate_to_stop;
    navigation::algorithm::TimeTableDijkstra navigator;
    // navigation::algorithm::TimeTable navigator;
};

} // namespace service
} // namespace nepomuk

#endif // NEPOMUK_SERVICE_ROUTE_HPP_
