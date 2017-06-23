#include "gtfs/agency.hpp"
#include "gtfs/constructor.hpp"

#include "date/timezone.hpp"

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
    auto const as_timezone_offset = [](std::string const& timezone){
        auto offset = date::Timezone::offset(timezone);
        BOOST_ASSERT(offset);
        return *offset;
    };

    return {
        construct<std::string>("agency_name", forward, header, values),
        construct<std::string>("agency_url", forward, header, values),
        construct<std::int32_t>("agency_timezone", as_timezone_offset, header, values),
        construct_as_optional<AgencyID, false>("agency_id", stringToID<AgencyID>, header, values),
        construct_as_optional<std::string, false>("agency_lang", forward, header, values),
        construct_as_optional<std::string, false>("agency_phone", forward, header, values),
        construct_as_optional<std::string, false>("agency_fare_url", forward, header, values),
        construct_as_optional<std::string, false>("agency_email", forward, header, values)};
}

} // namespace gtfs
} // namespace nepomuk
