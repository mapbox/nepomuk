#ifndef TRANSIT_ALGORITHM_GROUP_BY_HPP_
#define TRANSIT_ALGORITHM_GROUP_BY_HPP_

#include <algorithm>

#include <boost/assert.hpp>

namespace transit
{
namespace algorithm
{

// execute fn on all items that are considered equal in relation to predicate
// begin/end needs to be sorted based on `pred`
template <typename iterator_type, typename predicate, typename functor>
functor grouped_by(iterator_type begin, iterator_type end, predicate pred, functor fn)
{
    BOOST_ASSERT(std::is_sorted(begin, end, pred));
    while (begin != end)
    {
        auto range = std::equal_range(begin, end, *begin, pred);
        (void)fn(range);
        begin = range.second;
    }
    return fn;
}
}
} // transit

#endif // TRANSIT_ALGORITHM_GROUP_BY_HPP_
