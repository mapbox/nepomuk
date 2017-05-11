#ifndef NEPOMUK_GEOMETRIC_SHAPE_TABLE_HPP_
#define NEPOMUK_GEOMETRIC_SHAPE_TABLE_HPP_

#include "geometric/coordinate.hpp"

#include <boost/range/iterator_range>

#include <vector>

namespace nepomuk
{
namespace geometric
{

class ShapeTable
{
  public:
    using iterator = std::vector<WGS84Coordinate>::const_iterator;
    using iterator_range = boost::range::iterator_range<iterator>;

        iterator_range get(
    private:
        // the full shapes, ordered by their respective shape ID
        std::vector<std::vector<WGS84Coordinate>> shapes_by_id;
};

} // namespace geometric
} // namespace nepomuk

#endif // NEPOMUK_GEOMETRIC_SHAPE_TABLE_HPP_
