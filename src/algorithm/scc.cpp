#include "algorithm/scc.hpp"

namespace nepomuk
{
namespace algorithm
{

std::size_t StronglyConnectedComponent::size() const { return component_sizes.size(); }

std::size_t StronglyConnectedComponent::size(std::size_t const component_id) const
{
    return component_sizes[component_id];
}

std::uint64_t StronglyConnectedComponent::component(std::size_t const offset) const
{
    return node_to_component[offset];
}

} // namespace algorithm
} // namespace nepomuk
