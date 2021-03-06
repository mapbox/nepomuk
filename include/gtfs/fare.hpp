#ifndef NEPOMUK_GTFS_FARE_HPP_
#define NEPOMUK_GTFS_FARE_HPP_

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include "id/fare.hpp"
#include "id/route.hpp"
#include "id/zone.hpp"

namespace nepomuk
{
namespace gtfs
{

using CurrencyCode = std::string;

enum class PaymentType
{
    PRIOR_TO_BORDING,
    PAYABLE_ON_BOARD
};

enum class TransferPolicy
{
    FORBIDDEN,
    SINGLE,
    DOUBLE,
    UNLIMITED
};

struct FareAttribute
{
    FareID id;
    std::uint64_t price;
    CurrencyCode currency_type;
    PaymentType method;
    TransferPolicy transfer_policy;

    // transfer time in seconds
    boost::optional<std::uint64_t> transfer_duration;
};

bool checkFareAttributeCSVHeader(std::map<std::string, std::size_t> const &header);
FareAttribute makeFareAttribute(std::map<std::string, std::size_t> const &header,
                                std::vector<std::string> &values);

struct FareRule
{
    FareID id;

    boost::optional<RouteID> route_id;
    boost::optional<ZoneID> origin_id;
    boost::optional<ZoneID> destination_id;
    boost::optional<ZoneID> contains_id;
};

bool checkFareRuleCSVHeader(std::map<std::string, std::size_t> const &header);
FareRule makeFareRule(std::map<std::string, std::size_t> const &header,
                      std::vector<std::string> &values);

} // namespace gtfs
} // namespace nepomuk

#endif // NEPOMUK_GTFS_FARE_HPP_
