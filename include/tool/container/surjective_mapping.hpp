#ifndef NEPOMUK_TOOL_CONTAINER_SURJECTIVE_MAPPING_HPP_
#define NEPOMUK_TOOL_CONTAINER_SURJECTIVE_MAPPING_HPP_

#include <boost/range/iterator_range.hpp>
#include <cstddef>
#include <vector>

namespace nepomuk
{
namespace tool
{
namespace container
{

// a surjective mapping is a mapping that applies an image to all items in a codomain x into a
// domain y.
template <typename codomain_type, typename domain_type> class SurjectiveMapping
{
  public:
    using iterator = typename std::vector<codomain_type>::const_iterator;
    using iterator_range = typename boost::iterator_range<iterator>;

    SurjectiveMapping(std::size_t const codomain_size, std::size_t domain_size);

    // return the image of a codomain type
    domain_type codomain_mapping(codomain_type const value) const;

    // return the preimage range of a domain type
    iterator_range domain_mapping(domain_type const value) const;

    // add stuff to the mapping
    void map_codomain(codomain_type const from, domain_type const to);

    std::size_t codomain_size() const;
    std::size_t domain_size() const;

  private:
    // reverse mapping
    std::vector<std::vector<codomain_type>> domain_mappings;
    // forward mapping (surjectivity guarantees a single entry per domain type here)
    std::vector<domain_type> codomain_mappings;
};

template <typename codomain_type, typename domain_type>
domain_type
SurjectiveMapping<codomain_type, domain_type>::codomain_mapping(codomain_type const value) const
{
    return codomain_mappings[static_cast<std::uint64_t>(value)];
}

template <typename codomain_type, typename domain_type>
typename SurjectiveMapping<codomain_type, domain_type>::iterator_range
SurjectiveMapping<codomain_type, domain_type>::domain_mapping(domain_type const value) const
{
    return boost::make_iterator_range(domain_mappings[static_cast<std::uint64_t>(value)].begin(),
                                      domain_mappings[static_cast<std::uint64_t>(value)].end());
}

template <typename codomain_type, typename domain_type>
SurjectiveMapping<codomain_type, domain_type>::SurjectiveMapping(std::size_t const codomain_size,
                                                                 std::size_t domain_size)
    : domain_mappings(domain_size), codomain_mappings(codomain_size)
{
    // resizes the mappings
}

template <typename codomain_type, typename domain_type>
void SurjectiveMapping<codomain_type, domain_type>::map_codomain(codomain_type const from,
                                                                 domain_type const to)
{
    // add a new mapping to the domain
    codomain_mappings[static_cast<std::uint64_t>(from)] = to;
    domain_mappings[static_cast<std::uint64_t>(to)].push_back(from);
}

template <typename codomain_type, typename domain_type>
std::size_t SurjectiveMapping<codomain_type, domain_type>::domain_size() const
{
    return domain_mappings.size();
}

template <typename codomain_type, typename domain_type>
std::size_t SurjectiveMapping<codomain_type, domain_type>::codomain_size() const
{
    return codomain_mappings.size();
}

} // namespace container
} // namespace tool
} // namespace nepomuk

#endif // NEPOMUK_TOOL_CONTAINER_SURJECTIVE_MAPPING_HPP_
