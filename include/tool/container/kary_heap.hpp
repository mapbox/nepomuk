#ifndef TRANSIT_CONTAINER_SEARCH_HEAP_HPP_
#define TRANSIT_CONTAINER_SEARCH_HEAP_HPP_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <boost/heap/d_ary_heap.hpp>
#pragma GCC diagnostic pop

#include <boost/assert.hpp>

#include <unordered_map>
#include <utility>

namespace transit
{
namespace tool
{
namespace container
{

// a wrapper for the boost d-ary heap that allows decrase key operations to implement variations of
// Dijkstras algorithm
template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
class KAryHeap
{
    // define how to identify
    using HeapEntry = std::pair<key_type_t, value_type_t>;
    using HeapContainer = boost::heap::d_ary_heap<HeapEntry,
                                                  boost::heap::arity<arity>,
                                                  boost::heap::mutable_<true>,
                                                  boost::heap::compare<std::greater<HeapEntry>>>;
    // access into the heap to allow decreasing keys
    using HeapHandle = typename HeapContainer::handle_type;

    struct DataByValue
    {
        key_type_t key;
        HeapHandle handle;
        value_meta_data meta_data;
    };
    using DataContainer = std::unordered_map<value_type_t, DataByValue>;

  public:
    using key_type = key_type_t;
    using value_type = value_type_t;

    KAryHeap() {}

    void clear();

    bool empty() const;

    void insert(value_type const value, key_type const key, value_meta_data data);

    // mark element reached, but don't add it to the heap. Can be used to also decrease its key
    void reach_non_inserting(value_type const value, key_type const key, value_meta_data data);

    void decrease_key(value_type const value, key_type const key, value_meta_data data);

    bool contains(value_type const value) const;

    value_meta_data const &data(value_type const value) const;

    bool reached(const value_type value) const;

    key_type const &key(value_type value) const;

    key_type min_key() const;

    value_type min() const;

    value_type delete_min();

  private:
    DataContainer container;
    HeapContainer heap;
};

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
void KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::clear()
{
    heap.clear();
    container.clear();
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
bool KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::empty() const
{
    return heap.empty();
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
void KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::insert(value_type const value,
                                                                        key_type const key,
                                                                        value_meta_data data)
{
    auto handle = heap.push(std::make_pair(key, value));
    container[value] = {key, handle, std::move(data)};
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
void KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::reach_non_inserting(
    value_type const value, key_type const key, value_meta_data data)
{
    // add to the reached data, but not into the heap (non-transferring stop)
    container[value] = {key, HeapHandle(), std::move(data)};
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
void KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::decrease_key(
    value_type const value, key_type const key, value_meta_data data)
{
    auto reference = container.find(value);
    heap.increase(reference->second.handle, std::make_pair(key, value));
    reference->second.key = key;
    reference->second.meta_data = data;
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
bool KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::contains(
    value_type const value) const
{
    return container.find(value) != container.end() &&
           container.find(value)->second.handle != HeapHandle();
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
value_meta_data const &
KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::data(value_type const value) const
{
    BOOST_ASSERT(container.find(value) != container.end());
    return container.find(value)->second.meta_data;
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
bool KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::reached(
    const value_type value) const
{
    return container.find(value) != container.end();
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
key_type_t const &
KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::key(value_type_t value) const
{
    return container.find(value)->second.key;
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
key_type_t KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::min_key() const
{
    BOOST_ASSERT(!heap.empty());
    return heap.top().first;
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
value_type_t KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::min() const
{
    BOOST_ASSERT(!heap.empty());
    return heap.top().second;
}

template <typename value_type_t, typename key_type_t, int arity, typename value_meta_data>
value_type_t KAryHeap<value_type_t, key_type_t, arity, value_meta_data>::delete_min()
{
    BOOST_ASSERT(!heap.empty());

    auto const value = heap.top().second;
    heap.pop();
    container[value].handle = HeapHandle();

    return value;
}

} // namespace container
} // namespace tool
} // namespace transit

#endif // TRANSIT_CONTAINER_SEARCH_HEAP_HPP_
