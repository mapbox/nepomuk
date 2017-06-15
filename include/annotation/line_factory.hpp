#ifndef NEPOMUK_ANNOTATION_LINE_FACTORY_HPP_
#define NEPOMUK_ANNOTATION_LINE_FACTORY_HPP_

#include <vector>

#include "annotation/line.hpp"

namespace nepomuk
{

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
    static Line produce(std::vector<size_t> const &trip_offset_by_line,
                        std::vector<gtfs::Route> const &route,
                        std::vector<gtfs::Trip> const &trips);
};

} // namespace annotation
} // namespace nepomuk

#endif // NEPOMUK_ANNOTATION_LINE_FACTORY_HPP_
