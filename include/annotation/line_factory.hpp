#ifndef NEPOMUK_ANNOTATION_LINE_FACTORY_HPP_
#define NEPOMUK_ANNOTATION_LINE_FACTORY_HPP_

#include <vector>

#include "annotation/line.hpp"

namespace nepomuk
{

struct TripID;
namespace gtfs
{
struct Trip;
struct Route;
} // namespace gtfs

namespace annotation
{

// gather information to allow annotation of lines in gtfs feeds via the different other annotation
// tools (API/PBF)
class LineFactory
{
  public:
    static Line produce(std::vector<gtfs::Route> const &route,
                        std::vector<gtfs::Trip> const &trips,
                        std::vector<TripID> const &internal_to_external_trip_id);
};

} // namespace annotation
} // namespace nepomuk

#endif // NEPOMUK_ANNOTATION_LINE_FACTORY_HPP_
