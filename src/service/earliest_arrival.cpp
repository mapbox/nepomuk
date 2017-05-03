#include "service/earliest_arrival.hpp"

namespace transit
{
namespace service
{

EarliestArrival::EarliestArrival(service::Master &master_service)
    : annotation(master_service.api_annotation()),
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

    auto const get_locations = [this](auto const from, double radius) {
        auto result = coordinate_to_stop.all(from, radius);
        while (result.empty() && radius <= 1000)
        {
            radius *= 2;
            result = coordinate_to_stop.all(from, radius);
        }
        return result;
    };

    auto const from = get_locations(earliest_arrival_parameters.origin(), 150);
    auto const to = get_locations(earliest_arrival_parameters.destination(), 150);

    using input_type = navigation::RoutingAlgorithm::ADLeg;
    std::vector<input_type> origin;
    std::vector<input_type> destination;

    auto const make_converter = [](auto const base_coordinate) {
        return [base_coordinate](auto const &pair) -> input_type {
            auto const distance = geometric::distance(base_coordinate, pair.second);
            return {pair.first, static_cast<std::uint32_t>(std::llround(2 * distance)), distance};
        };
    };

    std::transform(from.begin(),
                   from.end(),
                   std::back_inserter(origin),
                   make_converter(earliest_arrival_parameters.origin()));
    std::transform(to.begin(),
                   to.end(),
                   std::back_inserter(destination),
                   make_converter(earliest_arrival_parameters.destination()));

    auto const route = [&]() -> boost::optional<navigation::Route> {
        if (earliest_arrival_parameters.departure())
        {
            return navigator(earliest_arrival_parameters.departure()->seconds_since_midnight(),
                             origin,
                             destination);
        }
        else
            return boost::none;
    }();

    if (route)
    {
        std::vector<navigation::Route> routes;
        routes.push_back(*route);
        earliest_arrival_parameters._result = annotation(routes);
    }
    else
    {
        const constexpr auto NO_ROUTE = "{\"code\": \"NoRoute\"}";
        earliest_arrival_parameters._result = NO_ROUTE;
    }

    return ServiceStatus::SUCCESS;
}

} // namespace service
} // namespace transit
