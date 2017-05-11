#ifndef NEPOMUK_GTFS_FEED_INFO_HPP_
#define NEPOMUK_GTFS_FEED_INFO_HPP_

#include <map>
#include <string>
#include <vector>

#include <boost/optional.hpp>

namespace nepomuk
{
namespace gtfs
{

struct FeedInfo
{
    std::string publisher;
    std::string publisher_url;
    std::string language;

    boost::optional<std::string> start_date;
    boost::optional<std::string> end_date;
    boost::optional<std::string> version;
};

bool checkFeedInfoCSVHeader(std::map<std::string, std::size_t> const &header);
FeedInfo makeFeedInfo(std::map<std::string, std::size_t> const &header,
                      std::vector<std::string> &values);

} // namespace gtfs
} // namespace nepomuk

#endif // NEPOMUK_GTFS_FEED_INFO_HPP_
