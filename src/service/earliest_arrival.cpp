#include "service/earliest_arrival.hpp"

namespace transit
{
namespace service
{

EarliestArrival::EarliestArrival(service::Master &master_service)
    : annotation(master_service.osrm_annotation()),
      coordinate_to_stop(master_service.coordinate_to_stop()),
      navigator(master_service.timetable(), master_service.stop_to_line())
{
}

ServiceStatus EarliestArrival::operator()(ServiceParameters &parameters) const
{
    auto &earliest_arrival_parameters =
        boost::get<EarliestArrivalParameters>(parameters.parameters);

    if (!earliest_arrival_parameters.valid())
        return ServiceStatus::ERROR;

    auto trip = navigator(earliest_arrival_parameters.departure(),
                          coordinate_to_stop.nearest(earliest_arrival_parameters.origin()),
                          coordinate_to_stop.nearest(earliest_arrival_parameters.destination()));

    const constexpr auto NO_ROUTE = "{\"code\": \"NoRoute\"}";

    if (trip)
        earliest_arrival_parameters._result = annotation(*trip);
    else
        earliest_arrival_parameters._result = NO_ROUTE;

    return ServiceStatus::SUCCESS;
}

} // namespace service
} // namespace transit
