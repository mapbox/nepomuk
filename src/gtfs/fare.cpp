#include "gtfs/fare.hpp"
#include "gtfs/constructor.hpp"

#include <algorithm>

#include <boost/optional.hpp>

namespace transit
{
namespace gtfs
{

bool checkFareAttributeCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("fare_id") && header.count("price") && header.count("currency_type") &&
           header.count("payment_method") && header.count("transfers");
}

FareAttribute makeFareAttribute(std::map<std::string, std::size_t> const &header,
                                std::vector<std::string> &values)
{
    const auto to_payment = [](std::string value) {
        return value == "0" ? PaymentType::PAYABLE_ON_BOARD : PaymentType::PRIOR_TO_BORDING;
    };

    const auto to_transfer = [](std::string value) {
        if (value == "0")
            return TransferPolicy::FORBIDDEN;
        else if (value == "1")
            return TransferPolicy::SINGLE;
        else if (value == "2")
            return TransferPolicy::DOUBLE;
        else
        {
            BOOST_ASSERT(value.empty());
            return TransferPolicy::UNLIMITED;
        }
    };

    return {construct<FareID>("fare_id", stringToID<FareID>, header, values),
            construct<std::uint64_t>("price", toInt, header, values),
            construct<std::string>("currency_code", forward, header, values),
            construct<PaymentType>("payment_method", to_payment, header, values),
            construct<TransferPolicy>("transfers", to_transfer, header, values),
            construct<boost::optional<std::uint64_t>>(
                "transfer_duration", asOptionalInt, header, values)};
}

bool checkFareRuleCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("fare_id");
}

FareRule makeFareRule(std::map<std::string, std::size_t> const &header,
                      std::vector<std::string> &values)
{
    return {
        construct<FareID>("fare_id", stringToID<FareID>, header, values),
        construct<boost::optional<RouteID>>(
            "route_id", stringToOptionalID<RouteID>, header, values),
        construct<boost::optional<StopID>>("origin_id", stringToOptionalID<StopID>, header, values),
        construct<boost::optional<StopID>>(
            "destination_id", stringToOptionalID<StopID>, header, values),
        construct<boost::optional<StopID>>(
            "contains_id", stringToOptionalID<StopID>, header, values)};
}

} // namespace gtfs
} // namespace transit
