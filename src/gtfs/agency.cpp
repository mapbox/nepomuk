#include "gtfs/agency.hpp"
#include "gtfs/constructor.hpp"

#include <algorithm>
#include <utility>

#include <boost/optional.hpp>

namespace transit
{
namespace gtfs
{

bool checkAgencyCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("agency_name") && header.count("agency_url") &&
           header.count("agency_timezone");
}

Agency makeAgency(std::map<std::string, std::size_t> const &header,
                  std::vector<std::string> &values)
{
    return {
        construct<std::string>("agency_name", forward, header, values),
        construct<std::string>("agency_url", forward, header, values),
        construct<std::string>("agency_timezone", forward, header, values),
        construct<boost::optional<std::string>>("agency_id", asOptionalString, header, values),
        construct<boost::optional<std::string>>("agency_lang", asOptionalString, header, values),
        construct<boost::optional<std::string>>("agency_phone", asOptionalString, header, values),
        construct<boost::optional<std::string>>(
            "agency_fare_url", asOptionalString, header, values),
        construct<boost::optional<std::string>>("agency_email", asOptionalString, header, values)};
}

} // namespace gtfs
} // namespace transit
