#ifndef NEPOMUK_TOOL_CONTAINER_INDEXED_VECTOR_HPP_
#define NEPOMUK_TOOL_CONTAINER_INDEXED_VECTOR_HPP_

#include "algorithm/functors.hpp"
#include <boost/range/iterator_range.hpp>
#include <cstddef>
#include <vector>

namespace nepomuk
{
namespace tool
{
namespace container
{

template <typename value_type> class IndexedVector
{
  public:
    // since older gccs cannot handle tuples from initialiser lists :(
    struct TwoDimensionalIndex
    {
        std::size_t category;
        std::size_t offset;
    };
    IndexedVector();

    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;
    using iterator_range = boost::iterator_range<iterator>;
    using const_iterator_range = boost::iterator_range<const_iterator>;

    void reserve(std::size_t const count);
    void push_back(std::size_t const category, const value_type &val);

    std::size_t category_size() const;
    std::size_t category_size(std::size_t const category) const;
    std::size_t data_size() const;

    value_type &operator[](std::size_t global_index);
    value_type const &operator[](std::size_t global_index) const;
    value_type &operator[](TwoDimensionalIndex category_and_index);
    value_type const &operator[](TwoDimensionalIndex category_and_index) const;

    // iterator access
    iterator begin();
    iterator end();
    iterator begin(std::size_t const category);
    iterator end(std::size_t const category);
    iterator_range range();
    iterator_range range(std::size_t const category);
    iterator_range range(std::size_t const category, std::size_t const from, std::size_t const to);

    // const iterator access
    const_iterator cbegin() const;
    const_iterator cend() const;
    const_iterator cbegin(std::size_t const category) const;
    const_iterator cend(std::size_t const category) const;
    const_iterator_range crange() const;
    const_iterator_range crange(std::size_t const category) const;
    const_iterator_range
    crange(std::size_t const category, std::size_t const from, std::size_t const to) const;

  private:
    std::vector<std::size_t> offsets;
    std::vector<value_type> data;
};

template <typename value_type>
IndexedVector<value_type>::IndexedVector() : offsets(1, 0) // sentinel
{
}

template <typename value_type> void IndexedVector<value_type>::reserve(std::size_t const count)
{
    data.reserve(count);
}

template <typename value_type>
void IndexedVector<value_type>::push_back(std::size_t const category, const value_type &val)
{
    // usually you don't want to add categories out of order, but you never know
    while (category + 1 >= offsets.size())
        offsets.push_back(data.size());

    // hopefully, this is the end of the vector. Otherwise this is quite costly
    data.insert(data.begin() + offsets[category + 1], val);
    std::for_each(offsets.begin() + category + 1,
                  offsets.end(),
                  algorithm::increase_by<std::size_t, std::size_t{1}>);
}

template <typename value_type> std::size_t IndexedVector<value_type>::category_size() const
{
    return offsets.size() - 1;
}

template <typename value_type>
std::size_t IndexedVector<value_type>::category_size(std::size_t category) const
{
    BOOST_ASSERT(category + 1 < offsets.size());
    return offsets[category + 1] - offsets[category];
}

template <typename value_type> std::size_t IndexedVector<value_type>::data_size() const
{
    return data.size() - 1;
}

template <typename value_type>
value_type &IndexedVector<value_type>::operator[](std::size_t global_index)
{
    return data[global_index];
}

template <typename value_type>
value_type const &IndexedVector<value_type>::operator[](std::size_t global_index) const
{
    return data[global_index];
}

template <typename value_type>
value_type &IndexedVector<value_type>::operator[](TwoDimensionalIndex category_and_index)
{
    return data[offsets[category_and_index.category] + category_and_index.offset];
}

template <typename value_type>
value_type const &IndexedVector<value_type>::
operator[](TwoDimensionalIndex category_and_index) const
{
    return data[offsets[category_and_index.category] + category_and_index.offset];
}

// iterator acces
template <typename value_type>
typename IndexedVector<value_type>::iterator IndexedVector<value_type>::begin()
{
    return data.begin();
}
template <typename value_type>
typename IndexedVector<value_type>::iterator IndexedVector<value_type>::end()
{
    return data.end();
}
template <typename value_type>
typename IndexedVector<value_type>::iterator
IndexedVector<value_type>::begin(std::size_t const category)
{
    return data.begin() + offsets[category];
}
template <typename value_type>
typename IndexedVector<value_type>::iterator
IndexedVector<value_type>::end(std::size_t const category)
{
    return data.begin() + offsets[category + 1];
}
template <typename value_type>
typename IndexedVector<value_type>::iterator_range IndexedVector<value_type>::range()
{
    return boost::make_iterator_range(data.begin(), data.end());
}

template <typename value_type>
typename IndexedVector<value_type>::iterator_range
IndexedVector<value_type>::range(std::size_t const category)
{
    return boost::make_iterator_range(begin(category), end(category));
}

template <typename value_type>
typename IndexedVector<value_type>::iterator_range IndexedVector<value_type>::range(
    std::size_t const category, std::size_t const from, std::size_t const to)
{
    return boost::make_iterator_range(begin(category) + from, begin(category) + to);
}

// const iterator acces
template <typename value_type>
typename IndexedVector<value_type>::const_iterator IndexedVector<value_type>::cbegin() const
{
    return data.cbegin();
}

template <typename value_type>
typename IndexedVector<value_type>::const_iterator IndexedVector<value_type>::cend() const
{
    return data.cend();
}

template <typename value_type>
typename IndexedVector<value_type>::const_iterator
IndexedVector<value_type>::cbegin(std::size_t const category) const
{
    return data.cbegin() + offsets[category];
}

template <typename value_type>
typename IndexedVector<value_type>::const_iterator
IndexedVector<value_type>::cend(std::size_t const category) const
{
    return data.cbegin() + offsets[category + 1];
}

template <typename value_type>
typename IndexedVector<value_type>::const_iterator_range IndexedVector<value_type>::crange() const
{
    return boost::make_iterator_range(data.cbegin(), data.cend());
}

template <typename value_type>
typename IndexedVector<value_type>::const_iterator_range
IndexedVector<value_type>::crange(std::size_t const category) const
{
    return boost::make_iterator_range(cbegin(category), cend(category));
}

template <typename value_type>
typename IndexedVector<value_type>::const_iterator_range IndexedVector<value_type>::crange(
    std::size_t const category, std::size_t const from, std::size_t const to) const
{
    return boost::make_iterator_range(cbegin(category) + from, cbegin(category) + to);
}

} // namespace container
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_CONTAINER_INDEXED_VECTOR_HPP_
