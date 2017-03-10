#ifndef TRANSIT_ANNOTATION_STOP_INFO_HPP_
#define TRANSIT_ANNOTATION_STOP_INFO_HPP_

#include "geometric/coordinate.hpp"
#include "gtfs/stop.hpp"
#include "tool/container/dictionary.hpp"

#include <vector>

namespace transit
{
namespace annotation
{

struct StopInfo
{
    tool::container::DictionaryID name_id;
    geometric::Coordinate location;
};

class StopInfoTable
{
  public:
    StopInfoTable(std::vector<gtfs::Stop> &stops);

    StopInfo get_info(gtfs::StopID const stop_id) const;

  private:
    std::vector<StopInfo> stop_info;
};

} // namespace annotation
} // namespace transit

#endif // TRANSIT_ANNOTATION_STOP_INFO_HPP_
