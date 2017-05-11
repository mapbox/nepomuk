#ifndef NEPOMUK_GTFS_FREQUENCY_HPP_
#define NEPOMUK_GTFS_FREQUENCY_HPP_

#include <boost/optional.hpp>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "date/time.hpp"
#include "id/trip.hpp"

namespace nepomuk
{
namespace gtfs
{

enum class FrequencyType
{
    INEXACT,
    EXACT
};

struct Frequency
{
    TripID trip_id;
    date::Time begin;
    date::Time end;
    std::uint64_t headway; // in seconds

    boost::optional<FrequencyType> type;
};

bool checkFrequencyCSVHeader(std::map<std::string, std::size_t> const &header);
Frequency makeFrequency(std::map<std::string, std::size_t> const &header,
                        std::vector<std::string> &values);

} // namespace gtfs
} // namespace nepomuk

#endif // NEPOMUK_GTFS_FREQUENCY_HPP_
