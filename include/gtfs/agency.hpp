#ifndef TRANSIT_GTFS_AGENCY_HPP_
#define TRANSIT_GTFS_AGENCY_HPP_

#include "tool/types_and_traits/strong_typedef.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <boost/optional.hpp>

namespace transit
{
namespace gtfs
{

STRONG_ID_TYPE(std::uint64_t, AgencyID)

struct Agency
{
    std::string name;
    std::string url;
    std::string timezone;
    boost::optional<AgencyID> id;
    boost::optional<std::string> language;
    boost::optional<std::string> phone;
    boost::optional<std::string> fare_url;
    boost::optional<std::string> email;
};

bool checkAgencyCSVHeader(std::map<std::string, std::size_t> const &header);
Agency makeAgency(std::map<std::string, std::size_t> const &header,
                  std::vector<std::string> &values);

} // namespace gtfs
} // namespace transit

// needs to be outside of namespaces
STRONG_ID_TYPE_HASH(std::uint64_t, transit::gtfs::AgencyID)
#endif // TRANSIT_GTFS_AGENCY_HPP_
