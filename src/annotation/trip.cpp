#include "annotation/trip.hpp"

#include <sstream>

namespace transit
{
namespace annotation
{

Trip::Trip(StopInfoTable const &stop_info, transit::tool::container::StringTable const &dictionary)
    : stop_info(stop_info), dictionary(dictionary)
{
}

// annotate a trip with all required information. This should be replaced with a json result/what
// ever we want for our api.
std::string Trip::operator()(navigation::Trip const &trip) const
{
    std::ostringstream oss;

    // Write out information about a trip
    for (auto const &leg : trip.list())
    {
        oss << "[Leg]\n";
        for (auto const &hold : leg.list())
        {
            auto const info = stop_info.get_info(hold.stop_id);
            oss << "\t" << (leg.departure() + hold.delta_t) << " at stop: " << hold.stop_id
                << " name: " << dictionary.get_string(info.name_id)
                << " located at: " << info.location << "\n";
        }
    }

    return oss.str();
}

} // namespace annotation
} // namespace transit
