#ifndef TRANSIT_ALGORITHM_SCC_HPP_
#define TRANSIT_ALGORITHM_SCC_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stack>
#include <vector>

#include <boost/assert.hpp>

#include "tool/status/progress.hpp"

namespace transit
{
namespace algorithm
{

// the actual strongly connected components
class StronglyConnectedComponent
{
  public:
    // the size of the container / the number of components compute
    std::size_t size() const;
    // the size of a component (numbered from 0 to size()-1)
    std::size_t size(std::size_t const component_id) const;

    // the component of a node offset
    std::uint64_t component(std::size_t const offset) const;

    using ComponentID = std::uint32_t;

  private:
    std::vector<ComponentID> node_to_component;
    std::vector<std::size_t> component_sizes;

    template <typename graph_type> friend class TarjanSCCAlgorithm;
};

// implementation of tarjans algorithm to compute strongly connected components in a directed
// network. `graph_type` needs to supply `nodes` / `edges` lookup
template <typename graph_type> class TarjanSCCAlgorithm
{
  private:
    StronglyConnectedComponent::ComponentID const static constexpr UNSET_COMPONENT_ID =
        std::numeric_limits<StronglyConnectedComponent::ComponentID>::max();

    struct StackFrame
    {
        explicit StackFrame(std::size_t const node_offset, std::size_t const edge_offset)
            : node_offset(node_offset), edge_offset(edge_offset)
        {
        }
        std::size_t node_offset;
        std::size_t edge_offset;
    };

    struct SCCState
    {
        // the maximum reachable depth / used as invalid value
        auto const static constexpr MAXIMUM_DEPTH = std::numeric_limits<std::uint64_t>::max();

        SCCState() : depth(MAXIMUM_DEPTH), low_link(MAXIMUM_DEPTH), on_stack(false) {}
        SCCState(std::uint64_t depth) : depth(depth), low_link(depth), on_stack(true) {}

        // the depth within the dfs
        std::uint64_t depth;
        // the lowest index reachable by any node
        std::uint64_t low_link;
        // check if node is on the component_stack
        bool on_stack;
    };

  public:
    static StronglyConnectedComponent compute(const graph_type &graph);
};

// location for our static variable
template <typename graph_type>
StronglyConnectedComponent::ComponentID const TarjanSCCAlgorithm<graph_type>::UNSET_COMPONENT_ID;

template <typename graph_type>
std::uint64_t const TarjanSCCAlgorithm<graph_type>::SCCState::MAXIMUM_DEPTH;

template <typename graph_type>
StronglyConnectedComponent TarjanSCCAlgorithm<graph_type>::compute(const graph_type &graph)
{
    tool::status::FunctionTimingGuard scc_timing_guard("SCC computation");
    StronglyConnectedComponent component;
    BOOST_ASSERT(!graph.empty());
    component.node_to_component.resize(graph.size(), UNSET_COMPONENT_ID);

    // The following is a hack to distinguish between stuff that happens
    // before the recursive call and stuff that happens after. The differentiation is required due
    // to the iterative implementation of the dfs, rather than a stack based approach.
    std::stack<StackFrame> recursion_stack;
    std::stack<std::size_t> component_nodes;

    std::vector<SCCState> node_state(graph.size());

    // the id of the next scc
    StronglyConnectedComponent::ComponentID component_id = 0;
    std::uint64_t depth = 0;

    auto const depth_is_set = [](SCCState const &state) {
        return state.depth < SCCState::MAXIMUM_DEPTH;
    };

    auto const update_link = [](SCCState &state, std::uint64_t const low_link) {
        state.low_link = std::min(state.low_link, low_link);
    };

    for (auto const &node : graph.nodes())
    {
        // start a component search at any uncategorised node
        if (!depth_is_set(node_state[graph.offset(node)]))
        {
            auto const nid = graph.offset(node);
            node_state[nid] = SCCState(depth++);
            component_nodes.push(nid);
            recursion_stack.emplace(StackFrame(nid, 0));
        }

        while (!recursion_stack.empty())
        {
            auto current_frame = recursion_stack.top();

            auto const current_node_index = current_frame.node_offset;
            auto const current_edge_index = current_frame.edge_offset;

            auto edge_range = graph.edges(graph.node(current_node_index));
            for (auto itr = edge_range.begin() + current_edge_index; itr != edge_range.end();
                 ++itr, ++recursion_stack.top().edge_offset)
            {
                auto const neighbor = graph.offset(itr->target());
                if (!depth_is_set(node_state[neighbor]))
                {
                    // new node
                    node_state[neighbor] = SCCState(depth++);
                    component_nodes.push(neighbor);
                    recursion_stack.emplace(StackFrame(neighbor, 0));
                    break;
                }
                else if (node_state[neighbor].on_stack)
                {
                    update_link(node_state[current_node_index], node_state[neighbor].low_link);
                }
            }

            // we didn't recurse anymore, so this is where the recursive implementation is
            // backtracking in the dfs
            if (recursion_stack.top().node_offset == current_node_index)
            {
                recursion_stack.pop();

                // a component is strongly conneected if the low_link equals the index of the
                // component root. In the case we find such a situaiton, all nodes on the
                // component_nodes stack form a strongly connected component.
                if (node_state[current_node_index].low_link == node_state[current_node_index].depth)
                {
                    std::size_t node_offset = 0;
                    std::size_t component_size = 0;
                    do
                    {
                        node_offset = component_nodes.top();
                        component_nodes.pop();
                        node_state[node_offset].on_stack = false;
                        component.node_to_component[node_offset] = component_id;
                        ++component_size;
                    } while (current_node_index != node_offset);

                    component.component_sizes.emplace_back(component_size);

                    ++component_id;
                }
            }
        }
    }
    std::cout << "[components] " << component.size() << " components.\n";
    return component;
}

// helper function to allow computation without specifying the graph type
template <typename graph_type> StronglyConnectedComponent computeSCC(graph_type const &graph)
{
    return TarjanSCCAlgorithm<graph_type>::compute(graph);
}

} // namespace algorithm
} // namespace transit

#endif // TRANSIT_ALGORITHM_SCC_HPP_
