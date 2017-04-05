#ifndef TRANSIT_ANNOTATION_STOP_INFO_HPP_
#define TRANSIT_ANNOTATION_STOP_INFO_HPP_

#include "geometric/coordinate.hpp"
#include "gtfs/stop.hpp"
#include "id/dictionary.hpp"
#include "id/stop.hpp"

#include <vector>

namespace transit
{
namespace annotation
{

struct StopInfo
{
    DictionaryID name_id;
    geometric::WGS84Coordinate location;
};

class StopInfoTable
{
  public:
    StopInfoTable(std::vector<gtfs::Stop> &stops);

    StopInfo get_info(StopID const stop_id) const;

  private:
    std::vector<StopInfo> stop_info;
};

} // namespace annotation
} // namespace transit

#endif // TRANSIT_ANNOTATION_STOP_INFO_HPP_
