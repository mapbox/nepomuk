#ifndef TRANSIT_ALGORITHM_SCC_HPP_
#define TRANSIT_ALGORITHM_SCC_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stack>
#include <vector>

namespace transit
{
namespace algorithm
{

// implementation of tarjans algorithm to compute strongly connected components in a directed
// network. `graph_type` needs to supply `nodes` / `edges` lookup
template <typename graph_type> class StronglyConnectedComponent
{
  private:
    using ComponentID = std::uint32_t;
    ComponentID const constexpr UNSET_COMPONENT_ID = std::numeric_limits<ComponentID>::max();

    // access into the graphs nodes
    using node_iterator = typename graph_type::node_iterator;

    struct StackFrame
    {
        explicit StackFrame(node_iterator const node_offset, node_iterator const parent_node_offset)
            : node_offset(node_offset), parent_node_offset(parent_node_offset)
        {
        }
        node_iterator node_offset;
        node_iterator parent_node_offset;
    };

    struct SCCState
    {
        // the maximum reachable depth / used as invalid value
        auto const constexpr MAXIMUM_DEPTH = std::numeric_limits<std::uint64_t>::max();

        SCCState() : depth(MAXIMUM_DEPTH), low_link(MAXIMUM_DEPTH), on_stack(false) {}
        SCCState(std::uint64_t depth) : depth(depth), low_link(depth), on_stack(true) {}

        // the depth within the dfs
        std::uint64_t depth;
        // the lowest index reachable by any node
        std::uint64_t low_link;
        // check if node is on the component_stack
        bool on_stack;
    };

    // compute the strongly connected components of "graph" using Tarjans algorithm
    void compute_components(graph_type const &graph);

    std::vector<ComponentID> node_to_component;
    std::vector<std::size_t> component_sizes;

  public:
    StronglyConnectedComponent(const graph_type &graph);

    // the size of the container / the number of components compute
    std::size_t size() const { return component_sizes.size(); }
    // the size of a component (numbered from 0 to size()-1)
    std::size_t size(std::size_t const component_id) const { return component_sizes[component_id]; }

    // the component of a node offset
    std::uint64_t component(std::size_t const offset) const { return node_to_component[node]; }
};

template <typename graph_type>
StronglyConnectedComponent<graph_type>::StronglyConnectedComponent(const graph_type &graph)
    : node_to_component(graph.size(), UNSET_COMPONENT_ID)
{
    BOOST_ASSERT(!graph.empty());
    compute_components(graph);
}

template <typename graph_type>
void StronglyConnectedComponent<graph_type>::compute_components(graph_type const &graph)
{
    // The following is a hack to distinguish between stuff that happens
    // before the recursive call and stuff that happens after
    std::stack<StackFrame> recursion_stack;
    std::stack<std::size_t> component_nodes;

    std::vector<SCCState> node_state(graph.size());

    // the id of the next scc
    ComponentID component_id = 0;
    std::uint64_t depth = 0;

    // since we implement the dfs via our own stack, not backing up much state, we need to ensure
    // not to do false processing. By using this array, we mark which kind of processing we are
    // looking at (pre recurse/post recurse)
    std::vector<bool> process_before_recursion(graph.size(), true);

    auto const depth_is_set = [](SCCState const &state) {
        return state.depth < SCCState::MAXIMUM_DEPTH;
    };

    auto const update_link = [](SCCState &state, std::uint64_t const depth) {
        state.low_link = std::min(low_link, depth);
    };

    for (auto const &node : graph.nodes())
    {
        // start a component search at any uncategorised node
        if (node_to_component[node] == UNSET_COMPONENT_ID)
            recursion_stack.emplace(StackFrame(graph.offset(node), graph.offset(node)));

        while (!recursion_stack.empty())
        {
            auto current_frame = recursion_stack.top();
            auto const current_parent = current_frame.parent_node_offset;
            auto const current_node_index = current_frame.node_offset;

            const bool before_recursion = process_before_recursion[current_node_index];

            if (before_recursion && depth_is_set(node_state[graph.offset(current_node)]))
            {
                recursion_stack.pop();
                continue;
            }

            if (before_recursion)
            {
                // Mark frame to handle tail of recursion
                process_before_recursion[current_node_index] = false;

                // Mark essential information for SCC
                node_state[current_node_index] = SCCState(depth++);
                component_nodes.push(current_node_index);

                for (auto const &edge : graph.edges(current_node))
                {
                    auto const neighbor_index = graph.index(edge.target());

                    if (node_state[neighbor_index].on_stack)
                        update_link(node_state[current_node], node_state[neighbor_index].depth);
                    else
                        recursion_stack.emplace(
                            TarjanStackFrame(neighbor_index, current_node_index));
                }
            }
            else
            {
                recursion_stack.pop();
                process_before_recursion[current_node_index] = true;
                update_link(node_state[current_parent_index],
                            node_state[current_node_index].low_link);

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
                        node_to_component[node_offset] = component_id;
                        ++component_size;
                    } while (current_node_index != node_offset);

                    component_sizes.emplace_back(component_size);

                    ++component_id;
                }
            }
        }
    }
}

} // namespace algorithm
} // namespace transit

#endif // TRANSIT_ALGORITHM_SCC_HPP_
