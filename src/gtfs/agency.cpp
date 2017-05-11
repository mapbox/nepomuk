#include "gtfs/agency.hpp"
#include "gtfs/constructor.hpp"

namespace nepomuk
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
        construct_as_optional<AgencyID, false>("agency_id", stringToID<AgencyID>, header, values),
        construct_as_optional<std::string, false>("agency_lang", forward, header, values),
        construct_as_optional<std::string, false>("agency_phone", forward, header, values),
        construct_as_optional<std::string, false>("agency_fare_url", forward, header, values),
        construct_as_optional<std::string, false>("agency_email", forward, header, values)};
}

} // namespace gtfs
} // namespace nepomuk
