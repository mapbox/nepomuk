#ifndef TRANSIT_GTFS_FARE_HPP_
#define TRANSIT_GTFS_FARE_HPP_

#include <cstdint>
#include <map>
#include <string>

#include <boost/optional.hpp>

#include "gtfs/route.hpp"
#include "gtfs/stop.hpp"
#include "tool/types_and_traits/strong_typedef.hpp"

namespace transit
{
namespace gtfs
{

STRONG_ID_TYPE(std::uint64_t, FareID)

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
    FareID fare_id;
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
    FareID fare_id;

    boost::optional<RouteID> route_id;
    boost::optional<StopID> origin_id;
    boost::optional<StopID> destination_id;
    boost::optional<StopID> contains_id;
};

bool checkFareRuleCSVHeader(std::map<std::string, std::size_t> const &header);
FareRule makeFareRule(std::map<std::string, std::size_t> const &header,
                      std::vector<std::string> &values);

} // namespace gtfs
} // namespace transit

// needs to be outside of namespaces
STRONG_ID_TYPE_HASH(std::uint64_t, transit::gtfs::FareID)
#endif // TRANSIT_GTFS_FARE_HPP_
