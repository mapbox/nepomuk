#ifndef TRANSIT_ANNOTATION_STOP_INFO_HPP_
#define TRANSIT_ANNOTATION_STOP_INFO_HPP_

#include "gtfs/stop.hpp"
#include "id/dictionary.hpp"
#include "id/stop.hpp"

#include <vector>

namespace transit
{
namespace annotation
{

// requires additional information like wheelchair access/transfers...
struct StopInfo
{
    DictionaryID name_id;
};

class StopInfoTable
{
  public:
    StopInfoTable(std::vector<gtfs::Stop> const &stops);
    StopInfo const &get_info(StopID const stop_id) const;

  private:
    std::vector<StopInfo> stop_info;
};

} // namespace annotation
} // namespace transit

#endif // TRANSIT_ANNOTATION_STOP_INFO_HPP_
