#ifndef TRANSIT_ALGORITHM_GROUP_BY_HPP_
#define TRANSIT_ALGORITHM_GROUP_BY_HPP_

#include <algorithm>
#include <functional>

#include <boost/assert.hpp>

namespace transit
{
namespace algorithm
{

// execute fn on all items that are considered equal in relation to predicate
// begin/end needs to be sorted based on `pred`
template <typename iterator_type, typename predicate, typename functor>
functor by_equal_ranges(iterator_type begin, iterator_type end, predicate pred, functor fn)
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

// call functions on consecutive elements that fulfill / not fullfill a predicate
// e.g. {0,0,0,1,1,3,2,2,2,0,1,1} and the predicate even would result in
// {0,0,0}, {1,1,3}, {2,2,2,0}, {1,1} to be used as ranges
template <typename iterator_type, typename predicate, typename functor>
functor on_alternating_ranges(iterator_type begin, iterator_type end, predicate pred, functor fn)
{
    if (begin == end)
        return fn;

    auto inverted = !pred(*begin);

    while (begin != end)
    {
        auto next = [&]() {
            if (inverted)
                return std::find_if(begin, end, pred);
            else
                return std::find_if_not(begin, end, pred);

        }();
        (void)fn(std::make_pair(begin, next));
        begin = next;
        inverted = !inverted;
    }
    return fn;
}

// find ranges that fulfill a predicatei
// e.g. {0,0,0,1,1,3,2,2,2,0,1,1} and the predicate even would result in
// {0,0,0}, {2,2,2,0} to be used as ranges
template <typename iterator_type, typename predicate, typename functor>
functor on_ranges(iterator_type begin, iterator_type end, predicate pred, functor fn)
{
    begin = std::find_if(begin, end, pred);
    while (begin != end)
    {
        const auto next = std::find_if_not(begin, end, pred);
        (void)fn(std::make_pair(begin, next));
        begin = std::find_if(next, end, pred);
    }
    return fn;
}

} // namespace algorithm
} // namespace transit

#endif // TRANSIT_ALGORITHM_GROUP_BY_HPP_
