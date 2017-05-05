#include "gtfs/frequency.hpp"
#include "gtfs/constructor.hpp"

#include <cstddef>

#include <boost/assert.hpp>

namespace transit
{
namespace gtfs
{

bool checkFrequencyCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("trip_id") && header.count("start_time") && header.count("end_time") &&
           header.count("headway_secs");
}
Frequency makeFrequency(std::map<std::string, std::size_t> const &header,
                        std::vector<std::string> &values)
{
    const auto to_frequency_type = [](std::string const &value) {
        if (value == "1")
            return FrequencyType::EXACT;
        else
        {
            BOOST_ASSERT(value.empty() || value == "0");
            return FrequencyType::INEXACT;
        }
    };

    auto exact_times = construct<std::string>("exact_times", forward, header, values);

    return {construct<TripID>("trip_id", stringToID<TripID>, header, values),
            construct<date::Time>("start_time", constructFromString<date::Time>, header, values),
            construct<date::Time>("end_time", constructFromString<date::Time>, header, values),
            construct<std::uint64_t>("headway_secs", toInt, header, values),
            exact_times.empty() ? boost::none
                                : boost::optional<FrequencyType>(to_frequency_type(exact_times))};
}

} // namespace gtfs
} // namespace transit
