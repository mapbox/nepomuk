#include "service/route.hpp"
#include "navigation/route.hpp"

namespace nepomuk
{
namespace service
{

Route::Route(service::Master &master_service)
    : coordinate_to_stop(master_service.coordinate_to_stop()),
      navigator(master_service.timetable(), master_service.stop_to_line())
{
}

std::vector<navigation::Route> Route::operator()(RouteParameters &parameters) const
{
    if (!parameters.valid())
        return {};

    auto const get_locations = [this](auto const from, double radius) {
        auto result = coordinate_to_stop.all(from, radius);
        while (result.empty() && radius <= 1000)
        {
            radius *= 2;
            result = coordinate_to_stop.all(from, radius);
        }
        return result;
    };

    auto const from = get_locations(parameters.origin(), parameters.walking_radius());
    auto const to = get_locations(parameters.destination(), parameters.walking_radius());

    using input_type = navigation::RoutingAlgorithm::ADLeg;
    std::vector<input_type> origin;
    std::vector<input_type> destination;

    auto const make_converter = [](auto const base_coordinate) {
        return [base_coordinate](auto const &pair) -> input_type {
            auto const distance = geometric::distance(base_coordinate, pair.second);
            return {pair.first, static_cast<std::uint32_t>(std::llround(2 * distance)), distance};
        };
    };

    std::transform(
        from.begin(), from.end(), std::back_inserter(origin), make_converter(parameters.origin()));
    std::transform(to.begin(),
                   to.end(),
                   std::back_inserter(destination),
                   make_converter(parameters.destination()));

    auto const route = [&]() -> boost::optional<navigation::Route> {
        if (parameters.departure())
        {
            return navigator(parameters.departure()->seconds_since_midnight(), origin, destination);
        }
        else
            return boost::none;
    }();

    std::vector<navigation::Route> routes;
    if (route)
        routes.push_back(*route);

    return routes;
}

} // namespace service
} // namespace nepomuk
