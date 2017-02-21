#include "gtfs/feed_info.hpp"
#include "gtfs/constructor.hpp"

#include <algorithm>
#include <boost/optional.hpp>

namespace transit
{
namespace gtfs
{

bool checkFeedInfoCSVHeader(std::map<std::string, std::size_t> const &header)
{
    return header.count("feed_publisher_name") && header.count("feed_publisher_url") &&
           header.count("feed_lang");
}

FeedInfo makeFeedInfo(std::map<std::string, std::size_t> const &header,
                      std::vector<std::string> &values)
{
    return {construct<std::string>("feed_publisher_name", forward, header, values),
            construct<std::string>("feed_publisher_url", forward, header, values),
            construct<std::string>("feed_lang", forward, header, values),
            construct<boost::optional<std::string>>("feed_start_date", forward, header, values),
            construct<boost::optional<std::string>>("feed_end_date", forward, header, values),
            construct<boost::optional<std::string>>("feed_version", forward, header, values)};
}

} // namespace gtfs
} // namespace transit
